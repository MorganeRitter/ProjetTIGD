#ifndef TOS_H
#define TOS_H

#include "pqueue.h"
#include "svm_img.h"
#include <vector>

template <typename T>
class TOS
{
public:
    TOS();
    std::vector<SVMCell<T>*> sort(const SVMImage<T> &img);

private:
};

#endif // TOS_H