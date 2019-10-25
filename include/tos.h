#ifndef TOS_H
#define TOS_H

#include "pqueue.h"
#include "svm_img.h"
#include <vector>

template <typename T>
class TOS
{
public:
    TOS(SVMImage<T> &img);

    // R comes from sort
    SVMCell<T> *unionFind(std::vector<SVMCell<T> *> R);
    SVMCell<T> *findRoot(SVMCell<T> *current);
    std::vector<SVMCell<T> *> computeTree();
    std::vector<SVMCell<T> *> sort();

private:
    SVMImage<T> &m_image;
};

#include "tos.hpp"

#endif // TOS_H