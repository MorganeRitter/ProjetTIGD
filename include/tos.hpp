#include "tos.h"

template <typename T>
TOS<T>::TOS(SVMImage<T> &img) : m_image(img)
{
    sortedPixels = sort();
    std::cout << "sort" << std::endl;
    unionFind();
    std::cout << "union find" << std::endl;
    canonize();
    std::cout << "canonize" << std::endl;
}

template <typename T>
void TOS<T>::unionFind()
{
    //std::cout << "Taille" << m_image.width() << " " << m_image.height() << '\n';
    for (int i = sortedPixels.size() - 1; i >= 0; i--)
    {
        SVMCell<T> *currentP = sortedPixels[i];
        currentP->parent(currentP);
        currentP->zpar(currentP);

        std::vector<SVMCell<T> *> neighbours;

        for (int j = currentP->posY() - 1; j <= currentP->posY() + 1; j++)
        {
            if (j < 0 || j >= m_image.height())
            {
                continue;
            }

            for (int k = currentP->posX() - 1; k <= currentP->posX() + 1; k++)
            {
                if ((j != currentP->posY() || k != currentP->posX()) && k >= 0 && k < m_image.width())
                {
                    neighbours.push_back(m_image(k, j));
                }
            }
        }

        for (auto neighbour : neighbours)
        {
            if (neighbour->zpar() != nullptr)
            {
                SVMCell<T> *root = findRoot(neighbour);
                if (root != currentP)
                {
                    root->parent(currentP);
                    root->zpar(currentP);
                }
            }
        }
    }
}
template <typename T>
SVMCell<T> *TOS<T>::findRoot(SVMCell<T> *current)
{
    if (current->zpar() == current)
    {
        return current;
    }
    else
    {
        current->zpar(findRoot(current->zpar()));
        return current->zpar();
    }
}

template <typename T>
std::vector<SVMCell<T> *> TOS<T>::sort()
{
    PQueue<T> q;
    std::vector<SVMCell<T> *> order;

    std::vector<SVMCell<T> *> neighbours;
    SVMCell<T> *n;

    // get first level
    SVMCell<T> *borderFace = m_image(0, 0); // p_infinite
    T borderValue = borderFace->value();    // l_infinite
    std::size_t initialLevel = static_cast<std::size_t>(borderValue);

    std::size_t l = initialLevel;

    q.push(borderFace, borderValue);

    while (!q.empty())
    {
        SVMCell<T> *currentFace = q.priority_pop(&l); // h

        currentFace->visited(true);
        currentFace->level(l);
        order.push_back(currentFace);

        // get neighbours
        for (int j = currentFace->posY() - 1; j <= currentFace->posY() + 1; j++)
        {
            if (j < 0 || j >= m_image.height())
            {
                continue;
            }

            for (int k = currentFace->posX() - 1; k <= currentFace->posX() + 1; k++)
            {
                if ((j != currentFace->posY() || k != currentFace->posX()) && k >= 0 && k < m_image.width())
                {

                    n = m_image(k, j);

                    if (!n->visited())
                    {
                        neighbours.push_back(n);
                    }
                }
            }
        }

        // add neighbourhood to queue
        for (unsigned int j = 0; j < neighbours.size(); j++)
        {
            q.priority_push(neighbours[j], l);
            neighbours[j]->visited(true);
        }
        neighbours.clear();
    }

    return order;
}

template <typename T>
void TOS<T>::canonize()
{
    for (auto current : sortedPixels)
    {
        while (current != current->parent())
        {
            // if parent is not original
            if (current->parent()->type() != CellType::Original)
                current->parent(current->parent()->parent());
            else // if it is original, move forward
                current = current->parent();
        }
    }

    // for all p in [R in reverse order]
#pragma omp parallel for
    for (long int i = sortedPixels.size() - 1; i >= 0; i--)
    {
        SVMCell<T> *p = sortedPixels[i];

        // q <- parent(p)
        SVMCell<T> *q = p->parent();

		std::vector<SVMCell<T> *> q_save;
		q_save.push_back(q);

		if(q->level() == p->level())
		{
			// if f(parent(q) == f(q)
	        while (q->parent()->level() == q->level() && q->parent() != q) // See svm_cell.h
	        {
				q = q->parent();
				q_save.push_back(q);
	            if (q->parent() == q)
	            {
	                p->parent(q->parent());
					for(auto current : q_save)
					{
						current->parent(q->parent());
					}
	            }
	        }

	        if (q->parent()->level() != q->level())
	        {
				p->parent(q->parent());
				for(auto current : q_save)
				{
					current->parent(q->parent());
				}
			}
		}
    }
}

template <typename T>
void TOS<T>::clean()
{
    sortedPixels.erase(std::remove_if(sortedPixels.begin(),sortedPixels.end(),[](SVMCell<T>*cell){return cell->type() != CellType::Original;}),sortedPixels.end());
}

sf::Color typeToColor(CellType type)
{
    switch (type)
    {
    case Original:
        return sf::Color::Red;
    case New:
        return sf::Color::Green;
    case Inter2:
    case Inter4:
        return sf::Color::Blue;
    }
}

template <typename T>
void TOS<T>::drawParents(sf::RenderWindow &window, const sf::Vector2f &pos)
{
    if (pos.x > 0 && pos.x < m_image.width() && pos.y > 0 && pos.y < m_image.height())
    {
        std::vector<sf::Vertex> path;
        sf::Vertex square[5];

        SVMCell<T> *cell = m_image(static_cast<std::size_t>(pos.x), static_cast<std::size_t>(pos.y));
        //std::cout << cell->posX() << " " << cell->posX() << std::endl;
        SVMCell<T> *current = cell;

        do
        {
            path.push_back(sf::Vertex(sf::Vector2f(current->posX() + 0.5f, current->posY() + 0.5f), typeToColor(current->type())));
            sf::Color col = typeToColor(current->type());
            square[0] = sf::Vertex(sf::Vector2f(current->posX(), current->posY()), col);
            square[1] = sf::Vertex(sf::Vector2f(current->posX() + 1, current->posY()), col);
            square[2] = sf::Vertex(sf::Vector2f(current->posX() + 1, current->posY() + 1), col);
            square[3] = sf::Vertex(sf::Vector2f(current->posX(), current->posY() + 1), col);
            square[4] = square[0];
            window.draw(square, 5, sf::LinesStrip);
            current = current->parent();
        } while (current->parent() != current);

        sf::Color col = typeToColor(cell->type());
        square[0] = sf::Vertex(sf::Vector2f(cell->posX(), cell->posY()), col);
        square[1] = sf::Vertex(sf::Vector2f(cell->posX() + 1, cell->posY()), col);
        square[2] = sf::Vertex(sf::Vector2f(cell->posX() + 1, cell->posY() + 1), col);
        square[3] = sf::Vertex(sf::Vector2f(cell->posX(), cell->posY() + 1), col);
        square[4] = square[0];

        window.draw(path.data(), path.size(), sf::LinesStrip);
        window.draw(square, 5, sf::LinesStrip);
    }
}
