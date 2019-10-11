#include "tos.h"

template <typename T>
TOS<T>::TOS(const SVMImage<T> &img) : m_image(img)
{
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
			m_image(currentP->x() - 1, currentP->y() - 1),
			m_image(currentP->x(), currentP->y() - 1),
			m_image(currentP->x() + 1, currentP->y() - 1),

			m_image(currentP->x() - 1, currentP->y()),
			m_image(currentP->x() + 1, currentP->y()),

			m_image(currentP->x() - 1, currentP->y() + 1),
			m_image(currentP->x(), currentP->y() + 1),
			m_image(currentP->x() + 1, currentP->y() + 1),
		};

		for (auto neighbour : neighbours)
		{
			if (neighbour->zpar() != nullptr)
			{
				SVMCell<T> root = findRoot(neighbour);
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
	T borderValue = borderFace.value();		// l_infinite
	std::size_t initialLevel = static_cast<std::size_t>(borderValue);

	unsigned int i = 0;
	std::size_t l = initialLevel;

	q.push(borderFace, borderValue);

	while (!q.empty())
	{
		SVMCell<T *> currentFace = q.priority_pop(l); // h
		currentFace.level(l);
		order.push_back(currentFace);

		// get neighbourhood
		for (unsigned int j = currentFace.y() - 1; j < currentFace.y() + 1; j++)
		{
			for (unsigned int k = currentFace.x() - 1; k < currentFace.x() + 1; k++)
			{

				n = m_image(j, k);
				if (!n.visited())
				{
					neighbours.push_back(n);
				}
			}
		}
		// add neighbourhood to queue
		for (unsigned int j = 0; j < neighbours.size(); j++)
		{
			q.priority_push(neighbours[j], l);
			neighbours[j].visited(true);
		}
		i++;
	}

	return order;
}