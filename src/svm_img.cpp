#include "svm_img.h"
#include <algorithm>
#include <vector>

template <typename T>
SVMImage<T>::SVMImage(const LibTIM::Image<T> &img) : m_original(img)
{
    m_width = img.getSizeX();
    m_height = img.getSizeY();
    extend();
    std::cout << "image extended" << std::endl;
    interpolate();
    std::cout << "image interpolated" << std::endl;
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

    std::vector<SVMCell<T>> e_img;

    for (LibTIM::TSize j = 0; j < newSizeY; ++j)
    {
        for (LibTIM::TSize i = 0; i < newSizeX; ++i)
        {
            if (i <= 0 || i >= newSizeX - 1 || j <= 0 || j >= newSizeY - 1)
            {
                SVMCell<T> cell(CellType::Original, median);
                e_img.push_back(cell);
            }
            else
            {
                SVMCell<T> cell(CellType::Original, m_original(i - 1, j - 1));
                e_img.push_back(cell);
            }
        }
    }
    std::cout << "e_img.size() = " << e_img.size() << std::endl;
    m_image = e_img;
    std::cout << "m_image.size() = " << m_image.size() << std::endl;
    m_width = newSizeX;
    m_height = newSizeY;
}

template <typename T>
void SVMImage<T>::interpolate()
{

    std::size_t n = m_width;
    std::size_t m = m_height;

    std::size_t nbCol = 2 * (n * 2 - 1) - 1;
    std::size_t nbLine = 2 * (m * 2 - 1) - 1;
    std::cout << nbCol << " " << nbLine << std::endl;
    std::vector<SVMCell<T>> i_img(nbLine * nbCol);

    std::cout << "initialized" << std::endl;
    // fill old pixels
    for (unsigned int l = 0; l < nbLine; l += 4)
    {
        for (unsigned int c = 0; c < nbCol; c += 4)
        {
            SVMCell<T> cell(CellType::Original, m_image.at((l / 4) * m_width + c / 4).value());
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l * nbCol + c) = cell;
        }
    }
    std::cout << "old pixels" << std::endl;

    // fill new pixels : lines
    for (unsigned int l = 0; l < nbLine; l += 4)
    {
        for (unsigned int c = 2; c < nbCol; c += 4)
        {
            SVMCell<T> cell(CellType::New, std::max(i_img.at(l * nbCol + c - 2).value(), i_img.at(l * nbCol + c + 2).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l * nbCol + c) = cell;
        }
    }
    std::cout << "New pixels" << std::endl;

    // Pas de out of range, on s'arrête à nbLine-2 puisqu'on fait c+=4 et qu'on commence à 2

    // fill new pixels : columns
    for (unsigned int c = 0; c < nbCol; c += 4)
    {
        for (unsigned int l = 2; l < nbLine; l += 4)
        {
            SVMCell<T> cell(CellType::New, std::max(i_img.at((l - 2) * nbCol + c).value(), i_img.at((l + 2) * nbCol + c).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l * nbCol + c) = cell;
        }
    }
    std::cout << "New pixels" << std::endl;

    // fill the remaining values
    // ordre de parcours arbitraire
    for (unsigned int l = 2; l < nbLine; l += 4)
    {
        for (unsigned int c = 2; c < nbCol; c += 4)
        {
            T m = std::max(std::max(i_img.at((l - 2) * nbCol + c).value(), i_img.at((l + 2) * nbCol + c).value()),
                           std::max(i_img.at(l * nbCol + c - 2).value(), i_img.at(l * nbCol + c + 2).value()));
            SVMCell<T> cell(CellType::New, m);
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l * nbCol + c) = cell;
        }
    }
    std::cout << "New pixels" << std::endl;

    // fill the interpixels

    for (unsigned int l = 0; l < nbLine; l += 2)
    {
        for (unsigned int c = 1; c < nbCol; c += 2)
        {
            SVMCell<T> cell(CellType::Inter2,
              std::min(i_img.at(l * nbCol + c - 1).value(), i_img.at(l * nbCol + c + 1).value()),
              std::max(i_img.at(l * nbCol + c - 1).value(), i_img.at(l * nbCol + c + 1).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l * nbCol + c) = cell;
        }
    }
    std::cout << "inter2 pixels" << std::endl;

    for (unsigned int c = 0; c < nbCol; c += 2)
    {
        for (unsigned int l = 1; l < nbLine; l += 2)
        {
            SVMCell<T> cell(CellType::Inter2,
              std::min(i_img.at((l - 1) * nbCol + c).value(), i_img.at((l + 1) * nbCol + c).value()),
              std::max(i_img.at((l - 1) * nbCol + c).value(), i_img.at((l + 1) * nbCol + c).value()));
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l * nbCol + c) = cell;
        }
    }
    std::cout << "inter2 pixels" << std::endl;

    // parcours arbitraire
    for (unsigned int l = 1; l < nbLine; l += 2)
    {
        for (unsigned int c = 1; c < nbCol; c += 2)
        {
            T ma = std::max(std::max(i_img.at((l - 1) * nbCol + c).value(), i_img.at((l + 1) * nbCol + c).value()),
                           std::max(i_img.at(l * nbCol + c - 1).value(), i_img.at(l * nbCol + c + 1).value()));
            T mi = std::min(std::min(i_img.at((l - 1) * nbCol + c).value(), i_img.at((l + 1) * nbCol + c).value()),
                          std::min(i_img.at(l * nbCol + c - 1).value(), i_img.at(l * nbCol + c + 1).value()));
            SVMCell<T> cell(CellType::Inter4, mi, ma);
            cell.posX(l);
            cell.posY(c);
            cell.visited(false);
            i_img.at(l * nbCol + c) = cell;
        }
    }
    std::cout << "inter4 pixels" << std::endl;

    m_image = i_img;
    m_height = nbLine;
    m_width = nbCol;
}

template <typename T>
const std::vector<SVMCell<T>> &SVMImage<T>::data() const
{
    return m_image;
}

template <typename T>
SVMCell<T> *SVMImage<T>::operator()(int i, int j)
{
    return &(m_image.at(m_height * i + j));
}

template <typename T>
SVMCell<T> &SVMImage<T>::get(int i, int j)
{
    return m_image.at(m_height * i + j);
}

template <typename T>
inline void SVMImage<T>::width(int w) { m_width = w; }
template <typename T>
void SVMImage<T>::height(int h) { m_height = h; }
template <typename T>
int SVMImage<T>::width() const { return m_width; }
template <typename T>
int SVMImage<T>::height() const { return m_height; }

template class SVMImage<int>;
template class SVMImage<unsigned int>;
template class SVMImage<char>;
template class SVMImage<unsigned char>;
template class SVMImage<long>;
template class SVMImage<unsigned long>;
template class SVMImage<float>;
template class SVMImage<double>;
