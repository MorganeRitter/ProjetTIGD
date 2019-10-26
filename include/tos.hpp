#include "tos.h"

template <typename T>
TOS<T>::TOS(SVMImage<T> &img) : m_image(img)
{
	std::vector<SVMCell<T> *> sortedPixels = sort();

    std::cout << "résultat du tri " << std::endl;
    for(unsigned int i = 0; i<sortedPixels.size(); i++) {
        std::cout
        << static_cast<unsigned int>(sortedPixels.at(i)->value()) << "("
        << static_cast<unsigned int>(sortedPixels.at(i)->posX()) << ","
        << static_cast<unsigned int>(sortedPixels.at(i)->posY()) << ")" << " " ;
    }
    std::cout << std::endl;
	SVMCell<T>* u = unionFind(sortedPixels);
}

template <typename T>
SVMCell<T> *TOS<T>::unionFind(std::vector<SVMCell<T> *> R)
{
	//std::cout << "Taille" << m_image.width() << " " << m_image.height() << '\n';
	for (int i = R.size() - 1; i >= 0; i--)
	{
		SVMCell<T> *currentP = R[i];
		currentP->parent(currentP);
		currentP->zpar(currentP);

		std::vector<SVMCell<T> *> neighbours;

		if(	currentP->posX() != 0
			&& currentP->posY() != 0
			&& currentP->posX() != m_image.width()-1
			&& currentP->posY() != m_image.height()-1)
		{
			//std::cout << currentP->posX() << " " << currentP->posY() << '\n';
			neighbours = {
				m_image(currentP->posX() - 1, currentP->posY() - 1),
				m_image(currentP->posX(), currentP->posY() - 1),
				m_image(currentP->posX() + 1, currentP->posY() - 1),

				m_image(currentP->posX() - 1, currentP->posY()),
				m_image(currentP->posX() + 1, currentP->posY()),

				m_image(currentP->posX() - 1, currentP->posY() + 1),
				m_image(currentP->posX(), currentP->posY() + 1),
				m_image(currentP->posX() + 1, currentP->posY() + 1),
			};
		}
		else if(currentP->posX() == 0 && currentP->posY() == 0) {
			//std::cout << "First line+col" << currentP->posX() << " " << currentP->posY() << '\n';
			neighbours = {
				m_image(currentP->posX() + 1, currentP->posY()),
				m_image(currentP->posX(), currentP->posY() + 1),
				m_image(currentP->posX() + 1, currentP->posY() + 1),
			};
		}
		else if(currentP->posX() == m_image.width()-1
				&& currentP->posY() == m_image.height()-1)
		{
			//std::cout << "Last line+col" << currentP->posX() << " " << currentP->posY() << '\n';
			neighbours = {
				m_image(currentP->posX() - 1, currentP->posY() - 1),
				m_image(currentP->posX(), currentP->posY() - 1),
				m_image(currentP->posX() - 1, currentP->posY()),
			};
		}
		else if(currentP->posX() == 0)
		{
			//std::cout <<"First col" << currentP->posX() << " " << currentP->posY() << '\n';
			neighbours = {
				m_image(currentP->posX(), currentP->posY() - 1),
				m_image(currentP->posX() + 1, currentP->posY() - 1),

				m_image(currentP->posX() + 1, currentP->posY()),

				m_image(currentP->posX(), currentP->posY() + 1),
				m_image(currentP->posX() + 1, currentP->posY() + 1),
			};
		}
		else if(currentP->posY() == 0)
		{
			//std::cout << "First line" << currentP->posX() << " " << currentP->posY() << '\n';
			neighbours = {
				m_image(currentP->posX() - 1, currentP->posY()),
				m_image(currentP->posX() + 1, currentP->posY()),

				m_image(currentP->posX() - 1, currentP->posY() + 1),
				m_image(currentP->posX(), currentP->posY() + 1),
				m_image(currentP->posX() + 1, currentP->posY() + 1),
			};
		}
		else if(currentP->posX() == m_image.width()-1)
		{
			//std::cout << "Last col" << currentP->posX() << " " << currentP->posY() << '\n';
			neighbours = {
				m_image(currentP->posX() - 1, currentP->posY() - 1),
				m_image(currentP->posX(), currentP->posY() - 1),

				m_image(currentP->posX() - 1, currentP->posY()),

				m_image(currentP->posX() - 1, currentP->posY() + 1),
				m_image(currentP->posX(), currentP->posY() + 1),
			};
		}
		else if(currentP->posY() == m_image.height()-1)
		{
			//std::cout << "Last line " << currentP->posX() << " " << currentP->posY() << '\n';
			neighbours = {
				m_image(currentP->posX() - 1, currentP->posY() - 1),
				m_image(currentP->posX(), currentP->posY() - 1),
				m_image(currentP->posX() + 1, currentP->posY() - 1),

				m_image(currentP->posX() - 1, currentP->posY()),
				m_image(currentP->posX() + 1, currentP->posY()),
			};
		}

		for (auto neighbour : neighbours)
		{
			if (neighbour->zpar() != nullptr)
			{
				SVMCell<T>* root = findRoot(neighbour);
				if (root == currentP)
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
std::vector<SVMCell<T> *> TOS<T>::computeTree()
{
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
	T borderValue = borderFace->value();		// l_infinite
	std::size_t initialLevel = static_cast<std::size_t>(borderValue);

	std::size_t l = initialLevel;

	q.push(borderFace, borderValue);

	while (!q.empty())
	{
		SVMCell<T> *currentFace = q.priority_pop(l); // h

		currentFace->visited(true);
		currentFace->level(l);
		order.push_back(currentFace);

		// get neighbours
        for (int j = currentFace->posY() - 1; j <= currentFace->posY() + 1; j++)
		{
			if(j < 0 || j >= m_image.height()) {
				continue;
			}

            for (int k = currentFace->posX() - 1; k <= currentFace->posX() + 1; k++)
			{
				if((j != currentFace->posY() || k != currentFace->posX()) && k >= 0 && k < m_image.width()) {

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
