#ifndef UTILS_H
#define UTILS_H

#include "svm_cell.h"
#include <SFML/Graphics.hpp>

static bool verbose = false;

#define VERBOSE(str)      \
    if (verbose)          \
    {                     \
        std::cout << str; \
    }

sf::Color typeToColor(CellType type)
{
    switch (type)
    {
    case Original:
        return sf::Color::Red;
    case New:
        return sf::Color::Green;
    case Inter2:
    case Inter4:
        return sf::Color::Blue;
    }
}

#endif // UTILS_H
