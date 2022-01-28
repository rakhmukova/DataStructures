#pragma once

#include <vector>
#include <initializer_list>

#include "Node.h"
#include "AVLIterator.h"
namespace avl{
    template<class TKey>
    class AVLTree {
    public:
        using TNode = Node<TKey>;
        Node<TKey> *root;

        static Node<TKey> *recoverBalance(Node<TKey> *subtree_root);

        static Node<TKey> *insert(Node<TKey> *subtree_root, const TKey &value);

        static Node<TKey> *findNode(const TKey &value, Node<TKey> *subtree_root);

        static Node<TKey> *deleteIfExists(const TKey &value, Node<TKey> *subtree_root);

        static Node<TKey> *leftRotate(Node<TKey> *k2);

        static Node<TKey> *rightRotate(Node<TKey> *k2);

        friend class AVLIterator<TKey>;

    public:
        AVLTree();

        explicit AVLTree(Node<TKey> *root);

        explicit AVLTree(const std::vector<TKey> &elements);

        AVLTree(const std::initializer_list<TKey> &il);

        AVLTree &operator=(const AVLTree &other);

        AVLTree(const AVLTree &other);

        ~AVLTree();

        size_t size() const { return root ? root->getSize() : 0; }

        size_t height() const { return root ? root->getHeight() : 0; }

        void insert(const TKey &value) { root = insert(root, value); }

        bool contains(const TKey &value) { return findNode(value, root) != nullptr; }

        bool deleteIfExists(const TKey &value) { return deleteIfExists(value, root) != nullptr; }

        void print(std::ostream &os) const;

        std::vector<TKey> toArray() const;

        static std::pair<TNode *, TNode *> split(TNode *subtree_root, const TKey &value, bool left_is_strictly_Less);

        static std::pair<AVLTree, AVLTree> split(AVLTree &tree, const TKey &value, bool left_is_strictly_Less);

        static TNode *mergeWithRootAndBalance(TNode *left, TNode *right, AVLTree::TNode *subtree_root);

        AVLTree<TKey> &removeMoreThan(const TKey &value); //redo and unify

        AVLTree<TKey> &removeLessThan(const TKey &value);

        AVLTree<TKey> &remainWithinBorders(const TKey &left_border, const TKey & right_border); //[...)
    };

    template <typename TKey>
    std::ostream& operator<<(std::ostream& os, const AVLTree<TKey> &tree);

    template <typename TKey>
    bool operator==(const AVLTree<TKey> &tree, const std::set<TKey> &set);

    template<class TKey>
    AVLTree<TKey> setIntersection(const AVLTree<TKey> &first, const AVLTree<TKey> &second);
    template<class TKey>
    AVLTree<TKey> setUnion(const AVLTree<TKey> &first, const AVLTree<TKey> &second);
    template<class TKey>
    AVLTree<TKey> setDifference(const AVLTree<TKey> &first, const AVLTree<TKey> &second);

    template<class TKey>
    bool operator ==(const AVLTree<TKey> &lhs, const AVLTree<TKey> &rhs);
    template<class TKey>
    bool operator !=(const AVLTree<TKey> &lhs, const AVLTree<TKey> &rhs);
}
