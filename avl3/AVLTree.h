#pragma once

#include <vector>
#include <initializer_list>
#include <functional>
#include <memory>
#include <cstdlib>
#include <stack>
#include <set>
#include <iostream>

namespace avl{
    template<class T, class TCompare = std::less<T>>
    class AVLTree {
    private:
        //node
        struct Node{
            using sNode = std::shared_ptr<Node>;

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

            void print(std::ostream &os, uint indent = 0) const;

            friend std::ostream& operator<<(std::ostream& os, const Node &node);

            sNode deepCopy() const;
        };

    public:
        //iterator
        struct AVLIterator{
        private:
            std::stack<typename Node::sNode> nodes;

            typename Node::sNode current;

            void pushLeftBranch(typename Node::sNode node);

        public:
            explicit AVLIterator(const AVLTree<T>& tree);

            T operator*() { return current->getValue(); }

            AVLIterator & operator=(const AVLIterator &other);

            AVLIterator & operator++();

            AVLIterator &operator++ (int);

            typename Node::sNode currentNode() { return current; } //redo
        };

        using sNode = std::shared_ptr<Node>;

        sNode root;

        TCompare cmp;

        static sNode recoverBalance(sNode subtree_root); //static vs non-static

        sNode insert(sNode &subtree_root, const T &value);

        sNode findNode(const T &value, sNode subtree_root);

        sNode deleteIfExists(const T &value, sNode &subtree_root);

        static sNode leftRotate(sNode k2);

        static sNode rightRotate(sNode k2);

    public:
        explicit AVLTree(sNode root = nullptr, const TCompare &cmp = TCompare());

        explicit AVLTree(const std::vector<T> &elements, const TCompare &cmp = TCompare());

        AVLTree(const std::initializer_list<T> &il, const TCompare &cmp = TCompare());

        AVLTree &operator=(const AVLTree &other);

        AVLTree(const AVLTree &other);

        size_t size() const { return root ? root->getSize() : 0; }

        size_t height() const { return root ? root->getHeight() : 0; }

        void insert(const T &value) { root = insert(root, value); }

        bool contains(const T &value) { return findNode(value, root) != nullptr; }

        bool deleteIfExists(const T &value) { return deleteIfExists(value, root) != nullptr; }

        void print(std::ostream &os) const;

        std::vector<T> toArray() const;

        static std::pair<sNode, sNode> split(sNode  subtree_root, const T &value, bool left_is_strictly_Less);

        static std::pair<AVLTree, AVLTree> split(AVLTree &tree, const T &value, bool left_is_strictly_Less);

        static sNode mergeWithRootAndBalance(sNode left, sNode right, sNode subtree_root);
    };

    template <typename T, typename TCompare>
    std::ostream& operator<<(std::ostream& os, const AVLTree<T, TCompare> &tree);

    template <typename T, typename TCompare>
    bool operator==(const AVLTree<T, TCompare> &tree, const std::set<T> &set);

    template<typename T, typename TCompare = std::less<T>>
    AVLTree<T, TCompare> setIntersection(const AVLTree<T, TCompare> &first, const AVLTree<T, TCompare> &second);

    template<typename T, typename TCompare = std::less<T>>
    AVLTree<T, TCompare> setUnion(const AVLTree<T, TCompare> &first, const AVLTree<T, TCompare> &second);

    template<typename T, typename TCompare = std::less<T>>
    AVLTree<T, TCompare> setDifference(const AVLTree<T, TCompare> &first, const AVLTree<T, TCompare> &second);

    template<typename T, typename TCompare>
    bool operator ==(const AVLTree<T, TCompare> &lhs, const AVLTree<T, TCompare> &rhs);

    template<typename T, typename TCompare>
    bool operator !=(const AVLTree<T, TCompare> &lhs, const AVLTree<T, TCompare> &rhs);
}