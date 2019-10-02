#ifndef PQUEUE_H
#define PQUEUE_H

#include "svm_cell.h"
#include <ostream>
#include <queue>
#include <vector>

template <typename T>
class PQueue
{
public:
    PQueue();

    void push(SVMCell<T> &cell, int level);
    SVMCell<T> &pop(int level);

    bool empty() const;

    friend std::ostream &operator<<(std::ostream &o, const PQueue &pq);

private:
    std::vector<std::queue<SVMCell<T> &>> m_pqueue;
};

#endif