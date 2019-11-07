#ifndef TOS_H
#define TOS_H

#include "pqueue.h"
#include "svm_img.h"
#include <SFML/Graphics.hpp>
#include <vector>

template <typename T>
class TOS
{
public:
    TOS(SVMImage<T> &img);

    // R comes from sort
    std::vector<SVMCell<T> *> sort();

    void unionFind();
    void canonize();

    void clean();

    void drawParents(sf::RenderWindow &window, const sf::Vector2f &pos);

private:
    SVMCell<T> *findRoot(SVMCell<T> *current);

    SVMImage<T> &m_image;
    std::vector<SVMCell<T> *> sortedPixels;
};

#include "tos.hpp"

#endif // TOS_H
