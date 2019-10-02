#include "svm_img.h"

template <typename T>
SVMImage<T>::SVMImage(const LibTIM::Image<T> &img) : m_image(img.getSizeX() * img.getSizeY())
{
}