#include "img_handler.h"

template <typename T>
ImgHandler<T>::ImgHandler(SVMImage<T> &img, int width, int height) : m_svmImage(img)
{
    m_image.create(m_svmImage.data().at(0).size(), m_svmImage.data().size());
    feed();
    m_texture.loadFromImage(m_image);
    m_sprite.setTexture(m_texture);
    float s = width / m_sprite.getGlobalBounds().height;
    //m_sprite.setScale(s, s);
}

template <typename T>
void ImgHandler<T>::draw(sf::RenderWindow &window)
{
    window.draw(m_sprite);
}

template <typename T>
void ImgHandler<T>::feed()
{
    int j = 0;
    for (auto row : m_svmImage.data())
    {
        int i = 0;
        for (SVMCell<T> cell : row)
        {
            if (cell.type() == CellType::Original || cell.type() == CellType::New)
            {
                sf::Uint8 val = static_cast<sf::Uint8>(cell.value());
                sf::Color col(val, val, val);
                m_image.setPixel(i, j, col);
            }
            else
            {
                sf::Uint8 val = static_cast<sf::Uint8>((cell.min() + cell.max()) / static_cast<T>(2));
                sf::Color col(val, val, val);
                m_image.setPixel(i, j, col);
            }
            i++;
        }
        j++;
    }
}

template class ImgHandler<int>;
template class ImgHandler<unsigned int>;
template class ImgHandler<char>;
template class ImgHandler<unsigned char>;
template class ImgHandler<long>;
template class ImgHandler<unsigned long>;
template class ImgHandler<float>;
template class ImgHandler<double>;