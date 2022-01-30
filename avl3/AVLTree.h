#pragma once

#include <vector>
#include <initializer_list>

#include "Node.h"
#include "AVLIterator.h"
namespace avl{
    template<class T>
    class AVLTree {
    private:
        using sNode = std::shared_ptr<Node<T>>;

        sNode root;

        static sNode recoverBalance(sNode subtree_root);

        static sNode insert(sNode &subtree_root, const T &value);

        static sNode findNode(const T &value, sNode subtree_root);

        static sNode deleteIfExists(const T &value, sNode &subtree_root);

        static sNode leftRotate(sNode k2);

        static sNode rightRotate(sNode k2);

        friend class AVLIterator<T>;

    public:
        explicit AVLTree(sNode root = sNode(nullptr));

        explicit AVLTree(const std::vector<T> &elements);

        AVLTree(const std::initializer_list<T> &il);

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

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const AVLTree<T> &tree);

    template <typename T>
    bool operator==(const AVLTree<T> &tree, const std::set<T> &set);

    template<class T>
    AVLTree<T> setIntersection(const AVLTree<T> &first, const AVLTree<T> &second);

    template<class T>
    AVLTree<T> setUnion(const AVLTree<T> &first, const AVLTree<T> &second);

    template<class T>
    AVLTree<T> setDifference(const AVLTree<T> &first, const AVLTree<T> &second);

    template<class T>
    bool operator ==(const AVLTree<T> &lhs, const AVLTree<T> &rhs);

    template<class T>
    bool operator !=(const AVLTree<T> &lhs, const AVLTree<T> &rhs);
}