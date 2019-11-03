#include "img_handler.h"
#include "pqueue.h"
#include "svm_cell.h"
#include "svm_img.h"
#include "tos.h"
#include <Common/Image.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>

void drawUI(sf::RenderWindow &window, const sf::View &view);

sf::Font font;

template <typename T>
void displaySVMImage(SVMImage<T> &img);

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout
        << "SVMCell<float>:" << sizeof(SVMCell<float>)
        << " | std::size_t:" << sizeof(std::size_t)
        << " | CellType:" << sizeof(CellType)
        << " | int:" << sizeof(int)
        << " | float:" << sizeof(float)
        << " | bool:" << sizeof(bool)
        << " | SVMCell*:" << sizeof(SVMCell<float> *)
        << std::endl;
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <image.pgm> \n";
        exit(1);
    }

    // Image est une classe générique paramétrée par le type des points contenus dans l'image
    LibTIM::Image<unsigned char> im;
    if (LibTIM::Image<LibTIM::U8>::load(argv[1], im))
        std::cout << "Great, image is loaded\n";
    else
        return 1;

    SVMImage<LibTIM::U8> svm_img(im);
    std::cout << "SVM object created\n";

    TOS<LibTIM::U8> tree(svm_img);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Tree computation executed in " << duration << " milliseconds" << std::endl;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tree of Shape", sf::Style::Default, settings);

    // A view is a "2D camera" that display a certain region of the 2D world. Moving or scaling a view
    // enables the user to navgate in the world.
    sf::View view(window.getView());

    // Needed to render the SVMImage on the screen (as a texture on a sprite)
    std::cout << "e_img.size() = " << svm_img.height() * svm_img.width() << " | " << svm_img.data().size() << std::endl;
    ImgHandler<LibTIM::U8> handler(svm_img);

    // Variables needed to compute mouse position changes (panning)
    sf::Vector2f newPos, oldPos;
    sf::Vector2f mousePos;

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
                    continue;
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

            if (event.type == sf::Event::MouseMoved)
            {
                mousePos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
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
        window.clear(sf::Color(255, 255, 255));
        handler.draw(window);
        tree.drawParents(window, mousePos);
        //tree.draw(window);
        drawUI(window, view);

        window.display();
    }

    return 0;
}

void drawUI(sf::RenderWindow &window, const sf::View &view)
{
    static bool initialized = false;
    if (!initialized)
    {
        font.loadFromFile("fonts/NotoMono-Regular.ttf");
        initialized = true;
    }
    float scale = view.getSize().x / static_cast<float>(window.getSize().x);
    //std::cout << view.getSize().x << " " << view.getSize().y << " " << scale << std::endl;
    sf::Vertex X[] = {
        sf::Vertex(sf::Vector2f(-(view.getSize().x - view.getCenter().x), 0.f), sf::Color::Red),
        sf::Vertex(sf::Vector2f((view.getSize().x + view.getCenter().x), 0.f), sf::Color::Red)};
    sf::Vertex Y[] = {
        sf::Vertex(sf::Vector2f(0.f, (view.getSize().y + view.getCenter().y)), sf::Color::Green),
        sf::Vertex(sf::Vector2f(0.f, -(view.getSize().y - view.getCenter().y)), sf::Color::Green)};

    /*int step = (scale + 50) / 100 * 100;
    for (int i = 0; i < 10; i++)
    {
        sf::Text text;
        text.setString(std::to_string(i * step));
        text.setFont(font);
        text.setCharacterSize(24);
        text.setColor(sf::Color::White);
        text.setScale({scale, scale});
        text.setPosition({i * step, (-24 * scale) - scale * 2.0f});
        window.draw(text);
    }*/

    window.draw(X, 2, sf::Lines);
    window.draw(Y, 2, sf::Lines);
}

template <typename T>
void displaySVMImage(SVMImage<T> &img)
{
    for (std::size_t i = 0; i < img.height(); i++)
    {
        for (std::size_t j = 0; j < img.width(); j++)
        {
            if (img(i, j)->type() == CellType::Original || img(i, j)->type() == CellType::New)
                std::cout << static_cast<unsigned int>(img(i, j)->value()) << " ";
            if (img(i, j)->type() == CellType::Inter2 || img(i, j)->type() == CellType::Inter4)
                std::cout << static_cast<unsigned int>(img(i, j)->min()) << " ";
        }
        std::cout << std::endl;
    }
}
