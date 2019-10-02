#include "pqueue.h"

template <typename T>
PQueue<T>::PQueue()
{
}

template <typename T>
void PQueue<T>::push(SVMCell<T> *cell, std::size_t level)
{
    // if the queue does not exist yet
    if (level >= m_pqueue.size())
    {
        for (std::size_t l = m_pqueue.size(); l < level + 1; l++)
        {
            m_pqueue.push_back(std::deque<SVMCell<T> *>());
        }
    }
    // push cell
    m_pqueue.at(level).push_back(cell);
}

template <typename T>
SVMCell<T> *PQueue<T>::pop(std::size_t level)
{
    // retrieve the front element
    SVMCell<T> *cell = m_pqueue.at(level).front();
    // delete the front element
    m_pqueue.at(level).pop_front();

    // delete the queue if it is last & empty
    if (m_pqueue.at(level).empty() && level == m_pqueue.size() - 1)
    {
        m_pqueue.pop_back();
    }
    clean();
    return cell;
}

template <typename T>
void PQueue<T>::priority_push(SVMCell<T> *cell, std::size_t level) {
    std::size_t lower;
    std::size_t upper;
    std::size_t levelToPush;

    if(cell->type() == CellType::Inter2 || cell->type() == CellType::Inter4) {
        lower = static_cast<std::size_t>(cell->min());
        upper = static_cast<std::size_t>(cell->max());
    } else {
        lower = static_cast<std::size_t>(cell->value());
        upper = static_cast<std::size_t>(cell->value());
    }

    if(lower > level) {
        levelToPush = lower;
    } else if (upper < level) {
        levelToPush = upper;
    } else {
        levelToPush = level;
    }

    push(cell, levelToPush);
}

template <typename T>
SVMCell<T> *PQueue<T>::priority_pop(std::size_t level) {
    std::size_t currentLevel = level;
    while(m_pqueue[currentLevel].size() == 0) {
        currentLevel = (currentLevel + 1) % m_pqueue.size();
    }
    return (pop(currentLevel));
}

template <typename T>
std::size_t PQueue<T>::size() const
{
    return m_pqueue.size();
}

template <typename T>
bool PQueue<T>::empty() const
{
    std::size_t i = m_pqueue.size() - 1;
    while (i >= 0)
    {
        if (!m_pqueue.at(i).empty())
        {
            return false;
        }
    }
    return true;
}

template <typename T>
bool PQueue<T>::levelIsEmpty(std::size_t level) const
{
    if (level < m_pqueue.size())
    {
        return m_pqueue.at(level).empty();
    }
    else
    {
        // level doesn't exist, therefore it is empty
        return true;
    }
}

template <typename T>
void PQueue<T>::clean()
{
    int i = m_pqueue.size() - 1;
    while (i >= 0 && m_pqueue.at(i).empty())
    {
        m_pqueue.pop_back();
        i--;
    }
}

template class PQueue<int>;
template class PQueue<unsigned int>;
template class PQueue<char>;
template class PQueue<unsigned char>;
template class PQueue<long>;
template class PQueue<unsigned long>;
template class PQueue<float>;
template class PQueue<double>;