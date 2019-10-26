#include "svm_img.h"
#include "utils.h"
#include <algorithm>
#include <vector>
#include <omp.h>
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

    std::size_t size = vec.size()-1;
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
    std::size_t size = nbCol*nbLine;
    std::cout << nbCol << " " << nbLine << std::endl;
    std::vector<SVMCell<T>> i_img(size);

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

    // fill in new pixels
#pragma omp parallel for
    for(std::size_t l = 0 ; l < nbLine ; l += 2)
    {
        //std::cout << omp_get_num_threads() << std::endl;

        for(std::size_t c = (l+2) % 4 ; c < nbCol; c += 4)
        {
            if(l % 4 == 2)
            {
                // max of both neighboor original pixels on the same column
                SVMCell<T> cell(CellType::New, std::max(i_img.at(clamp((l - 2) * nbCol + c,0ul,size-1)).value(),
                                                        i_img.at(clamp((l + 2) * nbCol + c,0ul,size-1)).value()));
                cell.posX(l);
                cell.posY(c);
                cell.visited(false);
                i_img.at(l * nbCol + c) = cell;

            }
            else if(l % 4 == 0)
            {
                // max of neighboor original pixel on the same line
                SVMCell<T> cell(CellType::New, std::max(i_img.at(clamp(l * nbCol + c - 2,0ul,size-1)).value(),
                                                        i_img.at(clamp(l * nbCol + c + 2,0ul,size-1)).value()));
                cell.posX(l);
                cell.posY(c);
                cell.visited(false);
                i_img.at(l * nbCol + c) = cell;
            }
        }
    }

    // fill the remaining values
    // ordre de parcours arbitraire
#pragma omp parallel for
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


#pragma omp parallel for
    for(std::size_t l = 0 ; l < nbLine ; l++)
    {
        //std::cout << omp_get_num_threads() << std::endl;

        for(std::size_t c = (l+1) % 2 ; c < nbCol; c+=2)
        {
            if(l % 2 == 1)
            {
                // max and min of both neighboor original or new pixels on the same column
                SVMCell<T> cell(CellType::Inter2,
					std::min(i_img.at(clamp((l - 1) * nbCol + c,0ul, size-1)).value(),
							i_img.at(clamp((l + 1) * nbCol + c,0ul,size-1)).value()),
					std::max(i_img.at(clamp((l - 1) * nbCol + c,0ul,size-1)).value(),
							i_img.at(clamp((l + 1) * nbCol + c,0ul,size-1)).value()));
                cell.posX(l);
                cell.posY(c);
                cell.visited(false);
                i_img.at(l * nbCol + c) = cell;

            }
            else if(l % 2 == 0)
            {
                // max and min of neighboor original or new pixel on the same line
                SVMCell<T> cell(CellType::Inter2,
					std::min(i_img.at(clamp(l * nbCol + c - 1,0ul,size-1)).value(),
							i_img.at(clamp(l * nbCol + c + 1,0ul,size-1)).value()),
					std::max(i_img.at(clamp(l * nbCol + c - 1,0ul,size-1)).value(),
							i_img.at(clamp(l * nbCol + c + 1,0ul,size-1)).value()));
                cell.posX(l);
                cell.posY(c);
                cell.visited(false);
                i_img.at(l * nbCol + c) = cell;
            }
        }
    }

    // parcours arbitraire
#pragma omp parallel for
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
    std::cout << "inter pixels" << std::endl;

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
SVMCell<T> *SVMImage<T>::operator()(std::size_t i, std::size_t j)
{
    return &(m_image.at(m_width * i + j));
}

template <typename T>
SVMCell<T> &SVMImage<T>::get(int i, int j)
{
    return m_image.at(m_width * i + j);
}

template <typename T>
inline void SVMImage<T>::width(std::size_t w) { m_width = w; }
template <typename T>
void SVMImage<T>::height(std::size_t h) { m_height = h; }
template <typename T>
std::size_t SVMImage<T>::width() const { return m_width; }
template <typename T>
std::size_t SVMImage<T>::height() const { return m_height; }
