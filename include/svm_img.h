#ifndef SVM_IMG_H
#define SVM_IMG_H

#include "svm_cell.h"
#include <Common/Image.h>
#include <vector>

template <typename T>
class SVMImage
{
public:
    SVMImage(const LibTIM::Image<T> &img);

    std::vector<SVMCell<T> &> &getByType(CellType type);

private:
    // add 1 pixel at the border of value median(Image)
    void extend();

    // interpolate the image
    void interpolate();

    // getter for a pixel @ pos [i,j]
    SVMCell<T> &operator()(int i, int j) const;

private:
    void constructSVMImage();
    std::vector<std::vector<SVMCell<T>>> m_image; // as [x,y]
    LibTIM::Image<T> m_original;
};

#endif