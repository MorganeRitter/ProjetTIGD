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

template <typename T>
void SVMImage<T>::interpolate()
{
    std::vector<std::vector<SVMCell<T>>> i_img;

    std::size_t n = m_image.size();
    std::size_t m = m_image.at(0).size();

    std::size_t nbCol = 2 * (n * 2 - 1) - 1;
    std::size_t nbLine = 2 * (m * 2 - 1) - 1;

    // fill old pixels
    for (unsigned int l = 0; l < nbLine; l += 4)
    {
        for (unsigned int c = 0; c < nbCol; c += 4)
        {
            SVMCell<T> cell(CellType::Original, m_image.at(l / 4).at(c / 4).value());
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l).at(c) = cell;
        }
    }

    // fill new pixels : lines
    for (unsigned int l = 0; l < nbLine; l += 4)
    {
        for (unsigned int c = 2; c < nbCol; c += 4)
        {
            SVMCell<T> cell(CellType::New, std::max(i_img.at(l).at(c - 2).value(), i_img.at(l).at(c + 2).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l).at(c) = cell;
        }
    }

    // Pas de out of range, on s'arrête à nbLine-2 puisqu'on fait c+=4 et qu'on commence à 2

    // fill new pixels : columns
    for (unsigned int c = 0; c < nbCol; c += 4)
    {
        for (unsigned int l = 2; l < nbLine; l += 4)
        {
            SVMCell<T> cell(CellType::New, std::max(i_img.at(l - 2).at(c).value(), i_img.at(l + 2).at(c).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l).at(c) = cell;
        }
    }

    // fill the remaining values
    // ordre de parcours arbitraire
    for (unsigned int l = 2; l < nbLine; l += 4)
    {
        for (unsigned int c = 2; c < nbCol; c += 4)
        {
            T m = std::max(std::max(i_img.at(l - 2).at(c).value(), i_img.at(l + 2).at(c).value()),
                           std::max(i_img.at(l).at(c - 2).value(), i_img.at(l).at(c + 2).value()));
            SVMCell<T> cell(CellType::New, m);
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l).at(c) = cell;
        }
    }

    // fill the interpixels

    for (unsigned int l = 0; l < nbLine; l += 2)
    {
        for (unsigned int c = 1; c < nbCol; c += 2)
        {
            SVMCell<T> cell(CellType::Inter2, std::max(i_img.at(l).at(c - 1).value(), i_img.at(l).at(c + 1).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l).at(c) = cell;
        }
    }

    for (unsigned int c = 0; c < nbCol; c += 2)
    {
        for (unsigned int l = 1; l < nbLine; l += 2)
        {
            SVMCell<T> cell(CellType::Inter2, std::max(i_img.at(l - 1).at(c).value(), i_img.at(l + 1).at(c).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l).at(c) = cell;
        }
    }

    // parcours arbitraire
    for (unsigned int l = 1; l < nbLine; l += 2)
    {
        for (unsigned int c = 1; c < nbCol; c += 2)
        {
            T m = std::max(std::max(i_img.at(l - 1).at(c).value(), i_img.at(l + 1).at(c).value()),
                           std::max(i_img.at(l).at(c - 1).value(), i_img.at(l).at(c + 1).value()));
            SVMCell<T> cell(CellType::Inter4, m);
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l).at(c) = cell;
        }
    }

    m_image = i_img;
}

template <typename T>
SVMCell<T> &SVMImage<T>::operator()(int i, int j) const
{
    return m_image.at(i).at(j);
}

template class SVMImage<int>;
template class SVMImage<unsigned int>;
template class SVMImage<char>;
template class SVMImage<unsigned char>;
template class SVMImage<long>;
template class SVMImage<unsigned long>;
template class SVMImage<float>;
template class SVMImage<double>;
