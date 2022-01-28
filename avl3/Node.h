#pragma once
#include <iostream>
#include <algorithm>
#include <set>
#include <functional>

namespace avl{
    template<class T>
    class AVLIterator;

    template<class TKey>
    class AVLTree;

    template <typename T>
    class Node{
    private:
        T value;
        Node<T> *left_child = nullptr;
        Node<T> *right_child = nullptr;
        unsigned int subtree_height = 1;
        size_t subtree_size = 1;

    public:
        explicit Node(const T &value, Node<T> *left_child = nullptr,
                      Node<T> *right_child = nullptr);
        Node &operator=(const Node &other);
        Node(const Node &other);
        ~Node();

        bool hasLeftChild() const noexcept{ return left_child!= nullptr; }
        bool hasRightChild() const noexcept{ return right_child!= nullptr;}

        const T &getValue() const noexcept { return value; }
        unsigned int getHeight() const noexcept { return subtree_height; }
        size_t getSize() const noexcept { return subtree_size; }
        int heightDiff() const;

        void updateHeight();
        void updateSize();

        friend class AVLTree<T>;
        friend class AVLIterator<T>;
        void print(std::ostream &os, uint indent = 0) const;
        friend std::ostream& operator<<(std::ostream& os, const Node<T> &node);
        Node<T> *deepCopy() const;
    };

    /*template<typename T>
    void inOrder(Node<T> &node, std::function<void(const T &)> act);

    template<typename T, typename Action>
    void inOrder(Node<T> &node, Action act);*/
}