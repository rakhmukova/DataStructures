#pragma once

#include <stack>
#include "AVLTree.h"

namespace avl{
    template <typename T>
    class AVLIterator{
    private:
        std::stack<typename Node<T>::sNode> nodes;

        typename Node<T>::sNode current;

        void pushLeftBranch(typename Node<T>::sNode node);

    public:
        explicit AVLIterator(const AVLTree<T>& tree);

        T operator*() { return current->value; }

        AVLIterator<T> & operator=(const AVLIterator<T> &other);

        AVLIterator<T> & operator++();

        AVLIterator<T> &operator++ (int);

        typename Node<T>::sNode currentNode() { return current; } //redo
    };
}