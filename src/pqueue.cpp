#include "pqueue.h"
#include <sstream>

template <typename T>
PQueue<T>::PQueue()
{
}

template <typename T>
void PQueue<T>::push(SVMCell<T> &cell, int level)
{
    if (level >= m_pqueue.size)
    {
        for (int l = m_pqueue.size(); l < level + 1; l++)
        {
            m_pqueue.push_back(std::queue<SVMCell<T>>());
        }
    }
    // push cell
    m_pqueue.at(level).push(cell);
}

template <typename T>
SVMCell<T> &PQueue<T>::pop(int level)
{
    SVMCell<T> &cell = m_pqueue.at(level).pop();
    if (m_pqueue.at(level).empty() && level == m_pqueue.size() - 1)
    {
        // supprimer la queue
    }
}

template <typename T>
bool PQueue<T>::empty() const
{
}

template <typename T>
bool PQueue<T>::levelIsEmpty() const
{
}

template <typename T>
std::ostream &operator<<(std::ostream &o, const PQueue<T> &pq)
{
    std::stringstream ss;
    ss << "{";
    for (auto q : pq.m_pqueue)
    {
        ss << "[";
        for (int i = 0; i < q.size(); i++)
        {
            ss << q[i] << " ";
        }
        ss << "] ";
    }
    ss << "}";
    return o << ss.str();
}