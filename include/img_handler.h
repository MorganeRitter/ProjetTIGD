#ifndef IMG_HANDLER_H
#define IMG_HANDLER_H

#include "svm_img.h"
#include <SFML/Graphics.hpp>

template <typename T>
class ImgHandler
{
public:
    ImgHandler(SVMImage<T> &img, int width, int height);

    void draw(sf::RenderWindow &window);

private:
    void feed();
    SVMImage<T> &m_svmImage;
    sf::Image m_image;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};

#endif