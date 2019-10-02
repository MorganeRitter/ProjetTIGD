#include "svm_img.h"
#include <algorithm>
#include <vector>

template <typename T>
SVMImage<T>::SVMImage(const LibTIM::Image<T> &img) : m_original(img)
{
}

template <typename T>
void SVMImage<T>::extend()
{
    // compute meidan value of the image
    std::vector<T> vec(m_original.begin(), m_original.end());
    std::size_t size = vec.size();
    // we assume the image is not of size 0,0
    std::sort(vec.begin(), vec.end());
    T median;
    if (size % 2 == 0)
    {
        median = (vec[size / 2 - 1] + vec[size / 2]) / 2;
    }
    else
    {
        median = vec[size / 2];
    }

    // extends the image with the median value
    LibTIM::TSize newSizeX = m_original.getSizeX() + 2;
    LibTIM::TSize newSizeY = m_original.getSizeY() + 2;

    std::vector<std::vector<SVMCell<T>>> e_img;

    for (LibTIM::TSize i = 0; i < newSizeX; ++i)
    {
        e_img.push_back(std::vector<SVMCell<T>>());
        for (LibTIM::TSize j = 0; j < newSizeY; ++j)
        {
            if (i == 0 || i == newSizeX - 1 || j == 0 || j == newSizeY - 1)
            {
                SVMCell<T> cell(CellType::Original, median);
                e_img.at(i).push_back(cell);
            }
            else
            {
                SVMCell<T> cell(CellType::Original, m_original(i, j));
                e_img.at(i).push_back(cell);
            }
        }
    }
    m_image = e_img;
}

template class SVMImage<int>;
template class SVMImage<unsigned int>;
template class SVMImage<char>;
template class SVMImage<unsigned char>;
template class SVMImage<long>;
template class SVMImage<unsigned long>;
template class SVMImage<float>;
template class SVMImage<double>;