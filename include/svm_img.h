#ifndef SVM_IMG_H
#define SVM_IMG_H

#include "svm_cell.h"
#include <Common/Image.h>

template <typename T>
class SVMImage
{
public:
    SVMImage(const LibTIM::Image<T> &img);

private:
    void constructSVMImage();
    std::vector<SVMCell<T>> m_image
};

#endif