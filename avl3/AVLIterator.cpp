#include "AVLIterator.h"

using namespace avl;

template<typename T>
AVLIterator<T> &AVLIterator<T>::operator++(){
    if (current){
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
void AVLIterator<T>::pushLeftBranch(typename Node<T>::sNode current_node) {
    while(current_node){
        nodes.push(current_node);
        current_node = current_node->left_child;
    }

    if (nodes.empty()){
        current = nullptr;
    }
    else{
        current = nodes.top();
        nodes.pop();
    }
}