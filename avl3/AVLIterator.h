#pragma once

#include <stack>
#include "AVLTree.h"

namespace avl{
    template <typename T>
    class AVLIterator{
    private:
        std::stack<Node<T>*> nodes;
        void pushLeftBranch(Node<T> *node);
    public:
        explicit AVLIterator(const AVLTree<T>& tree);

        T operator*() { return nodes.top()->value; } //undefined behaviour ^_^
        T next() const { return nodes.top()->value; }
        //friend bool operator==(const AVLIterator<T>& other) const{ return *current == *other.current; }
        //friend bool operator!=(const AVLIterator<T>& other) const{ return !(*this == other); }
        AVLIterator<T> & operator=(const AVLIterator<T> &other);
        AVLIterator<T> & operator++();
        AVLIterator<T> &operator++ (int);
        bool hasNext() { return !nodes.empty(); }
    };
}



