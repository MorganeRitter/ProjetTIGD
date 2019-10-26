#include "tos.h"

template <typename T>
TOS<T>::TOS(SVMImage<T> &img) : m_image(img)
{
	std::vector<SVMCell<T> *> sortedPixels = sort();

    std::cout << "résultat du tri" << std::endl;
    for(unsigned int i = 0; i<sortedPixels.size(); i++) {
        std::cout    << static_cast<unsigned int>(sortedPixels.at(i)->value())
         << "("
            << static_cast<unsigned int>(sortedPixels.at(i)->posX()) << "," << static_cast<unsigned int>(sortedPixels.at(i)->posY()) << ")" << " " ;
    }
    std::cout << std::endl;
}

template <typename T>
SVMCell<T> *TOS<T>::unionFind(std::vector<SVMCell<T> *> R)
{
	for (int i = R.size() - 1; i >= 0; i--)
	{
		SVMCell<T> *currentP = R[i];
		currentP->parent(currentP);
		currentP->zpar(currentP);

		std::vector<SVMCell<T> *> neighbours = {
			m_image(currentP->posX() - 1, currentP->posY() - 1),
			m_image(currentP->posX(), currentP->posY() - 1),
			m_image(currentP->posX() + 1, currentP->posY() - 1),

			m_image(currentP->posX() - 1, currentP->posY()),
			m_image(currentP->posX() + 1, currentP->posY()),

			m_image(currentP->posX() - 1, currentP->posY() + 1),
			m_image(currentP->posX(), currentP->posY() + 1),
			m_image(currentP->posX() + 1, currentP->posY() + 1),
		};

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

	// int i =0;

	while (!q.empty())
	{
		// if(i == 100) { //71
		// 	std::cout << "tour de boucle" << std::endl;
		// 	return order;
		// 	// break;
		// }

		// i++;

		SVMCell<T> *currentFace = q.priority_pop(l); // h
		// std::cout << "face select : (" << currentFace->posX() << "," << currentFace->	posY() << ")" << std::endl;

		currentFace->visited(true);
		currentFace->level(l);
		order.push_back(currentFace);


		// std::cout << "found neighbours : " <<std::endl;;

		// get neighbours
        for (int j = currentFace->posY() - 1; j <= currentFace->posY() + 1; j++)
		{
			if(j < 0 || j >= m_image.height()) {
				continue;
			}

            for (int k = currentFace->posX() - 1; k <= currentFace->posX() + 1; k++)
			{
				if((j != currentFace->posY() || k != currentFace->posX()) && k >= 0 && k < m_image.width()) {

					// std::cout << "(" << k << "," << j << "), ";
					n = m_image(k, j);

					if(n->visited()) {
						// std::cout << "seen ";
					}

					// std::cout << "done" << std::endl;
					// std::cout << "image size is " << m_image.width() << ", " << m_image.height() << std::endl;

					if (!n->visited())
					{
						// std::cout << "ok ";
						neighbours.push_back(n);
					}
				}

				
			}
		}

		// std::cout << std::endl;

		// add neighbourhood to queue
		for (unsigned int j = 0; j < neighbours.size(); j++)
		{
			q.priority_push(neighbours[j], l);
			neighbours[j]->visited(true);
		}

		// std::cout << "queue is now " << q << std::endl;

		neighbours.clear();
	}

	return order;
}
