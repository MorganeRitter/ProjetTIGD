#ifndef SVM_IMG_H
#define SVM_IMG_H

#include "svm_cell.h"
#include <Common/Image.h>
#include <SFML/Graphics/Image.hpp>
#include <vector>

// A SVMImage represent a Set Value Map image and is composed of SVMCell.
template <typename T>
class SVMImage
{
public:
    SVMImage(const LibTIM::Image<T> &img);

    std::vector<SVMCell<T> &> &getByType(CellType type);
    // read only public acces to image data
    const std::vector<SVMCell<T>> &data() const;

    inline void width(int w);
    inline void height(int h);
    inline int width() const;
    inline int height() const;

private:
    // add 1 pixel at the border of value median(Image)
    void extend();

    // interpolate the image
    void interpolate();

    // getter for a pixel @ pos [i,j]
    SVMCell<T> &operator()(int i, int j);

    SVMCell<T> &get(int i, int j);
    void constructSVMImage();

private:
    int m_height, m_width;

    std::vector<SVMCell<T>> m_image;
    LibTIM::Image<T> m_original;
};

#endif