#include "pqueue.h"
#include "svm_cell.h"
#include "svm_img.h"
#include <Common/Image.h>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace LibTIM;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <image.pgm> \n";
        exit(1);
    }
    // Image est une classe générique paramétrée par le type des points contenus dans l'image
    Image<unsigned char> im;
    if (Image<U8>::load(argv[1], im))
        std::cout << "Great, image is loaded\n";
    else
        return 1;

    SVMImage<U8> svm_img(im);
    std::cout << "SVM object created\n";
    sf::Image img;
    img = svm_img.asSFImage();
    std::cout << " created sf::Image" << std::endl;
    sf::Texture tex;
    tex.loadFromImage(img);
    sf::Sprite sprite;
    sprite.setTexture(tex);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tree of Shape", sf::Style::Default, settings);

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                default:
                    break;
                }
            }
        }

        window.clear();
        window.draw(sprite);

        window.display();
    }

    return 0;
}