#pragma once
#include <iostream>
#include <algorithm>
#include <set>
#include <functional>
#include <memory>

namespace avl{
    template<class T>
    class AVLIterator;

    template<class T>
    class AVLTree;

    template <typename T>
    class Node{
    private:
        using sNode = std::shared_ptr<Node<T>>;

        T value;

        sNode left_child;

        sNode right_child;

        unsigned int subtree_height = 1;

        size_t subtree_size = 1;

    public:
        explicit Node(const T &value, const sNode &left_child = nullptr,
                      const sNode &right_child = nullptr);

        Node &operator=(const Node &other);

        Node(const Node &other);

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

        sNode deepCopy() const;
    };
}