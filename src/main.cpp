#include "pqueue.h"
#include "svm_cell.h"
#include <Common/Image.h>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace LibTIM;
using namespace std;

int main(int argc, char *argv[])
{

    PQueue<int> pq;

    pq.push(new SVMCell<int>(CellType::Original, 1), 2);
    pq.push(new SVMCell<int>(CellType::Original, 3), 0);
    pq.push(new SVMCell<int>(CellType::Original, 4), 0);
    pq.pop(0);

    std::cout << pq << " | " << (pq.empty() == true ? "true" : "false") << std::endl;

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <image.pgm> \n";
        //exit(1);
    }
    // Image est une classe générique paramétrée par le type des points contenus dans l'image
    Image<unsigned char> im;
    if (Image<U8>::load(argv[1], im))
        std::cout << "Great, image is loaded\n";
    //else
    //return 1;

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

        window.display();
    }

    return 0;
}