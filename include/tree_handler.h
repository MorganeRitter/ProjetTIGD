#ifndef TREE_HANDLER_H
#define TREE_HANDLER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "svm_cell.h"

template <typename T>
class Collection
{
public:
    Collection() {}
    std::vector<SVMCell<T>*> collection;
};

template <typename T>
class Node
{
public:
    Node() {}

    Collection<T> *m_parent;
    Collection<T> data;
    std::vector<Collection<T>*> m_children;
};

template <typename T>
class TreeHandler
{
public:
    TreeHandler();

    void draw(sf::RenderWindow &window);

    Node<T> root;

};

#include "tree_handler.hpp"

#endif // TREE_HANDLER_H
