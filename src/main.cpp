#include "img_handler.h"
#include "pqueue.h"
#include "svm_cell.h"
#include "svm_img.h"
#include <Common/Image.h>
#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <image.pgm> \n";
        exit(1);
    }

    // Image est une classe générique paramétrée par le type des points contenus dans l'image
    LibTIM::Image<unsigned char> im;
    if (LibTIM::Image<U8>::load(argv[1], im))
        std::cout << "Great, image is loaded\n";
    else
        return 1;

    SVMImage<LibTIM::U8> svm_img(im);
    std::cout << "SVM object created\n";

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tree of Shape", sf::Style::Default, settings);

    // A view is a "2D camera" that display a certain region of the 2D world. Moving or scaling a view
    // enables the user to navgate in the world.
    sf::View view(window.getView());

    // Needed to render the SVMImage on the screen (as a texture on a sprite)
    ImgHandler<LibTIM::U8> handler(svm_img, window.getSize().x, window.getSize().y);

    // Variables needed to compute mouse position changes (panning)
    sf::Vector2f newPos, oldPos;

    // Flag which indicates when the lef button is in the down position
    bool mouseButtonDown = false;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            // the user closes the window
            if (event.type == sf::Event::Closed)
                window.close();

            // a keyboard key is pressed
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

            // The window is resized
            if (event.type == sf::Event::Resized)
            {
                view.setSize(event.size.width, event.size.height);
                window.setView(view);
            }

            // A mouse button is pressed
            if (event.type == sf::Event::MouseButtonPressed)
            {
                // Left button
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    // The left button is now in the DOWN position
                    mouseButtonDown = true;
                    // Retrieving the mouse position at the event time
                    oldPos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                }
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    // Left button now in the UP position
                    mouseButtonDown = false;
                }
            }

            // Scrolling event ==> zoom in/out
            if (event.type == sf::Event::MouseWheelMoved)
            {
                float delta = event.mouseWheel.delta;
                if (delta > 0)
                    view.zoom(0.9f);
                else
                    view.zoom(1.1f);
                window.setView(view);
            }

            if (mouseButtonDown)
            {
                newPos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
                sf::Vector2f deltaPos = oldPos - newPos;
                // the view is moved by the vector representing the change in position of the mouse cursor
                view.setCenter(view.getCenter() + deltaPos);
                window.setView(view);
                oldPos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
            }
        }

        // Draw calls
        window.clear();
        handler.draw(window);

        window.display();
    }

    return 0;
}