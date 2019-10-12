#include "img_handler.h"
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

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tree of Shape", sf::Style::Default, settings);
    sf::View view(window.getView());

    ImgHandler<U8> handler(svm_img, window.getSize().x, window.getSize().y);

    sf::Vector2f newPos, oldPos;
    bool mouseButtonDown = false;

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
            if (event.type == sf::Event::Resized)
            {
                view.setSize(event.size.width, event.size.height);
                window.setView(view);
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
            {
                mouseButtonDown = true;
                oldPos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left)
            {
                mouseButtonDown = false;
            }
            if (mouseButtonDown)
            {
                newPos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
                sf::Vector2f deltaPos = oldPos - newPos;
                view.setCenter(view.getCenter() + deltaPos);
                window.setView(view);
                oldPos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                float delta = event.mouseWheel.delta;
                if (delta > 0)
                    view.zoom(0.9f);
                else
                    view.zoom(1.1f);
                window.setView(view);
            }
        }

        window.clear();
        handler.draw(window);
        window.display();
    }

    return 0;
}