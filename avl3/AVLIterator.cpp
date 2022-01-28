#include "AVLIterator.h"

using namespace avl;

template<typename T>
AVLIterator<T> &AVLIterator<T>::operator++(){
    if (!nodes.empty()){
        auto current = nodes.top();
        nodes.pop();
        pushLeftBranch(current->right_child);
    }

    return *this;
}

template<typename T>
AVLIterator<T> &AVLIterator<T>::operator++(int){
    AVLIterator<T> copy = *this;
    ++this;
    return copy;
}

template<typename T>
AVLIterator<T>::AVLIterator(const AVLTree<T>& tree){
    pushLeftBranch(tree.root);
}

template<typename T>
AVLIterator<T> &AVLIterator<T>::operator=(const AVLIterator<T> &other) {
    if (*this != other){
        this->nodes = other.nodes;
    }
    return *this;
}

template<typename T>
void AVLIterator<T>::pushLeftBranch(Node<T> *current_node) {
    while(current_node != nullptr){
        nodes.push(current_node); //copying constructor
        current_node = current_node->left_child;
    }
}
