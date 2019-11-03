#include "tos.h"

template <typename T>
TOS<T>::TOS(SVMImage<T> &img) : m_image(img)
{
	sortedPixels = sort();

	/*std::cout << "résultat du tri " << std::endl;
	for (unsigned int i = 0; i < sortedPixels.size(); i++)
	{
		std::cout
			<< static_cast<unsigned int>(sortedPixels.at(i)->value()) << "("
			<< static_cast<unsigned int>(sortedPixels.at(i)->posX()) << ","
			<< static_cast<unsigned int>(sortedPixels.at(i)->posY()) << ")"
			<< " ";
	}
	std::cout << std::endl;*/
	unionFind();
	/*for (unsigned int i = 0; i < sortedPixels.size(); i++)
	{
		std::cout << (sortedPixels.at(i)->parent() == sortedPixels.at(i) ? "same" : "diff") << std::endl;
	}
	std::cout << "done" << std::endl;*/
	canonize();
    /*for (unsigned int i = 0; i < sortedPixels.size(); i++)
	{
		std::cout << "[" << i << "] " << sortedPixels.at(i)->parent() << std::endl;
	}
    std::cout << "done" << std::endl;*/
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
	T borderValue = borderFace->value();	// l_infinite
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
	// for all p in [R in reverse order]
//#pragma omp parallel for
    for (long int i = sortedPixels.size() - 1; i >= 0; i--)
	{
		SVMCell<T> *p = sortedPixels[i];

		// q <- parent(p)
		SVMCell<T> *q = p->parent();

		// if f(parent(q) == f(q)
		if (areSameVal(*(q->parent()), *q)) // See svm_cell.h
		{
			//std::cout << "same val" << std::endl;
			// parent(p) <- parent(q)
			p->parent(q->parent());
		}
	}
}

template <typename T>
void TOS<T>::drawParents(sf::RenderWindow &window, const sf::Vector2f &pos)
{
	if (pos.x > 0 && pos.x < m_image.width() && pos.y > 0 && pos.y < m_image.height())
	{
		std::vector<sf::Vertex> vertices;
		SVMCell<T> *cell = m_image(static_cast<std::size_t>(pos.x), static_cast<std::size_t>(pos.y));
		vertices.push_back(sf::Vertex(sf::Vector2f(cell->posX(), cell->posY()), sf::Color::Red));
		SVMCell<T> *current = cell->parent();
		while (current->parent() != current)
		{
			vertices.push_back(sf::Vertex(sf::Vector2f(current->posX(), current->posY()), sf::Color::Green));
			current = current->parent();
		}
        window.draw(vertices.data(), vertices.size(), sf::LinesStrip);
	}
}
