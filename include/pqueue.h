#ifndef PQUEUE_H
#define PQUEUE_H

#include "svm_cell.h"
#include <deque>
#include <ostream>
#include <sstream>
#include <vector>
template <typename T>
class PQueue
{
public:
    PQueue();

    // push <cell> in queue at level <level>
    void push(SVMCell<T> *cell, std::size_t level);
    // pop the cell at <level> and returns it
    SVMCell<T> *pop(std::size_t level);

    // get the number of queues up to the last non-empty queue
    std::size_t size() const;
    // check if the PQueue is empty
    bool empty() const;
    // check if a certain queue in a PQueue is empty
    bool levelIsEmpty(std::size_t level) const;

    /*debug:
    Pqueue pq;
    std:cout << pqueue << std::endl;
    */
    friend std::ostream &operator<<(std::ostream &o, const PQueue &pq)
    {
        std::stringstream ss;
        ss << "{";
        for (auto q : pq.m_pqueue)
        {
            ss << "[";
            for (int i = 0; i < q.size(); i++)
            {
                ss << q[i]->value() << " ";
            }
            ss << "] ";
        }
        ss << "}";
        return o << ss.str();
    }

private:
    void clean();

    std::vector<std::deque<SVMCell<T> *>> m_pqueue;
};

#endif