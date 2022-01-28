#include <iostream>
#include "Node.h"

using namespace avl;

template<typename T>
Node<T> &Node<T>::operator =(const Node<T> &other){
    if (this != &other) {
        delete (this->left_child);
        delete (this->right_child);
        auto *tmp = new Node<T>(other);
        std::swap(*tmp, *this);
    }
    return *this;
}

template<typename T>
void Node<T>::updateHeight()
{
    this->subtree_height = 1 + std::max(left_child ? left_child->subtree_height : 0,
                                        right_child ? right_child->subtree_height : 0);
}

template<typename T>
void Node<T>::updateSize() {
    this->subtree_size = 1
                         + (left_child ? left_child->subtree_size : 0)
                         + (right_child ? right_child->subtree_size: 0);
}

template <typename T>
Node<T>::~Node() {
    delete left_child;
    left_child = nullptr; //redo
    delete right_child;
    right_child = nullptr;
}

template<typename T>
Node<T>::Node(const T &value, Node *left_child, Node *right_child)
        :value(value)
        ,left_child(left_child)
        ,right_child(right_child)
        { //in case it is created recursively
    updateHeight();
    updateSize();
}

template<typename T>
Node<T> *Node<T>::deepCopy() const{
    auto *copy = new Node<T>(this->value);
    if (right_child){
        copy->right_child = right_child->deepCopy();
    }
    if (left_child){
        copy->left_child = left_child->deepCopy();
    }
    copy->updateSize();
    copy->updateHeight();
    return copy;
}

template<typename T>
Node<T>::Node(const Node &other)
:value(value)
{
    if (other.left_child){
        left_child = new Node(*other.left_child);
    }

    if (other.right_child){
        right_child = new Node(*other.right_child);
    } //redo

    updateSize();
    updateHeight();
}