#include "AVLTree.h"

using namespace std;
using namespace avl;

template<typename T>
int Node<T>::heightDiff() const {
    int left_height = left_child? left_child->getHeight() : 0;
    int right_height = right_child ? right_child->getHeight() : 0;
    return left_height - right_height;
}

template<typename T>
void Node<T>::print(ostream &os, uint indent) const{
    auto child = this->right_child;
    if (child){
        child->print(os, indent + 3);
    }

    for (uint i = 0; i < indent; ++i){
        os << " ";
    }
    os << this->value << "(s" << this->subtree_size << ", h" << this->subtree_height << ')' << '\n';

    child = this->left_child;
    if (child){
        child->print(os, indent + 3);
    }
}

template <typename T>
std::ostream &operator<<(ostream &os, const Node<T> &node) {
    node.print(os);
    return os;
}

template<typename T>
typename AVLTree<T>::sNode AVLTree<T>::insert(sNode &subtree_root, const T &value) {
    if (!subtree_root){
        return make_shared<Node<T>>(value);
    }

    if (value == subtree_root->getValue()){
        return subtree_root;
    }

    if (value < subtree_root->getValue()){
        subtree_root->left_child = insert(subtree_root->left_child, value);
    }
    else {
        subtree_root->right_child = insert(subtree_root->right_child, value);
    }

    subtree_root = recoverBalance(subtree_root);
    return subtree_root;
}

template<class T>
typename AVLTree<T>::sNode AVLTree<T>::findNode(const T &value, sNode subtree_root) {
    if (subtree_root == nullptr || value == subtree_root->getValue()){
        return subtree_root;
    }

    if (value < subtree_root->getValue()){
        return findNode(value, subtree_root->left_child);
    }
    else{
        return findNode(value, subtree_root->right_child);
    }
}

template<typename T>
typename AVLTree<T>::sNode AVLTree<T>::deleteIfExists(const T &value, sNode &subtree_root) {
    if (subtree_root == nullptr){
        return nullptr;
    }

    if (value == subtree_root->getValue()){
        if (subtree_root->left_child == nullptr){
            subtree_root = subtree_root->right_child;
            return subtree_root;
        }

        sNode temp = subtree_root->left_child;
        while(temp->right_child){
            temp = temp->right_child;
        }
        std::swap(subtree_root->value, temp->value); //sizes and heights remain the same
        subtree_root->left_child = deleteIfExists(value, subtree_root->left_child);
    }
    else if(value < subtree_root->getValue()){
        subtree_root->left_child = deleteIfExists(value, subtree_root->left_child);
    }
    else{
        subtree_root->right_child = deleteIfExists(value, subtree_root->right_child);
    }

    subtree_root = recoverBalance(subtree_root);
    return subtree_root;
}

template<class T>
typename AVLTree<T>::sNode AVLTree<T>::leftRotate(sNode k2) {
    sNode k1 = k2->right_child;
    k2->right_child = k1->left_child;
    k1->left_child = k2;

    k2->updateHeight();
    k2->updateSize();
    k1->updateHeight();
    k1->updateSize();
    return k1;
}

/* leftRotate:
        k2                       k1
       /  \                     /  \
      X    k1         ==>      k2   Z
          /  \                /  \
         Y    Z              X    Y
 */

template<class T>
typename AVLTree<T>::sNode AVLTree<T>::rightRotate(sNode k2) {
    sNode k1 = k2->left_child;
    k2->left_child = k1->right_child;
    k1->right_child = k2;

    k2->updateHeight();
    k2->updateSize();
    k1->updateHeight();
    k1->updateSize();
    return k1;
}

/* rightRotate:
        k2                   k1
       /  \                 /  \
      k1   Z     ==>       X   k2
     / \                      /  \
    X   Y                    Y    Z
*/

template<class T>
typename AVLTree<T>::sNode AVLTree<T>::recoverBalance(sNode subtree_root) {
    int diff = subtree_root->heightDiff();
    if (diff >= -1 && diff <= 1){
        subtree_root->updateHeight();
        subtree_root->updateSize();
        return subtree_root;
    }

    if (diff == 2){ //left child exists
        if (subtree_root->left_child->heightDiff() < 0){ //>0
            subtree_root->left_child = leftRotate(subtree_root->left_child);
        }
        subtree_root = rightRotate(subtree_root);
    }
    else if (diff == - 2){ //right child exists
        if (subtree_root->right_child->heightDiff() > 0){
            subtree_root->right_child = rightRotate(subtree_root->right_child);
        }
        subtree_root = leftRotate(subtree_root);
    }

    return subtree_root;
}

template<class T>
AVLTree<T>::AVLTree(sNode root) //what's going on
        :root(root){}

template<class T>
AVLTree<T>::AVLTree(const vector<T> &elements)
:root(nullptr){
    for (auto el : elements){
        insert(el);
    }
}

template<class T>
std::vector<T> AVLTree<T>::toArray() const{
    vector<T> array;
    auto it = AVLIterator<T>(*this); //copying is occuring
    while (it.currentNode() != nullptr){
        array.push_back(*it);
        ++it;
    }
    return array;
}

template<class T>
AVLTree<T> &AVLTree<T>::operator=(const AVLTree &other) {
    if (this != &other){
        AVLTree<T>tmp(other);
        std::swap(*this, tmp);
    }
    return *this;
}

template<class T>
AVLTree<T>::AVLTree(const AVLTree &other){
    if (other.root){
        root = make_shared<Node<T>>(Node<T>(*other.root));
    }
}

/*
template<class T>
AVLTree<T>::~AVLTree() {
    //delete root;
}*/

template<class T>
typename AVLTree<T>::sNode
AVLTree<T>::mergeWithRootAndBalance(AVLTree::sNode  left, AVLTree::sNode  right, AVLTree::sNode  subtree_root) {
    if (!subtree_root){
        return nullptr;
    }
    //

    int heightDiff = (left? left->getHeight() : 0)
            - (right? right->getHeight() : 0);

    if (heightDiff <= 1 && heightDiff >= -1){
        subtree_root->left_child = left; //constructor if pass by ref
        subtree_root->right_child = right;
        subtree_root->updateHeight();
        subtree_root->updateSize();
        return subtree_root;
    }
    if (heightDiff > 1){
        auto temp_root = mergeWithRootAndBalance(left->right_child, right, subtree_root);
        left->right_child = temp_root;
        left = recoverBalance(left); //inside could be replaced
        return left;
    }

    auto temp_root = mergeWithRootAndBalance(left, right->left_child, subtree_root);
    right->left_child = temp_root;
    right = recoverBalance(right);
    return right;
}

template<class T>
pair<typename AVLTree<T>::sNode  , typename AVLTree<T>::sNode  >
AVLTree<T>::split(AVLTree::sNode  subtree_root, const T &value, bool left_is_strictly_Less) {
    std::pair<sNode  , sNode  > result(nullptr, nullptr);
    if (!subtree_root){
        return result;
    }
    //go left
    if (value < subtree_root->getValue() || value == subtree_root->getValue() && left_is_strictly_Less){
        auto nodes = split(subtree_root->left_child, value, left_is_strictly_Less);
        auto new_right = mergeWithRootAndBalance(nodes.second, subtree_root->right_child, subtree_root);
        return std::make_pair(nodes.first, new_right);
    } //go right
    else{
        auto nodes = split(subtree_root->right_child, value, left_is_strictly_Less);
        auto new_left = mergeWithRootAndBalance(subtree_root->left_child, nodes.first, subtree_root);
        return std::make_pair(new_left, nodes.second);
    }
}

template<class T>
void AVLTree<T>::print(ostream &os) const {
    os << "===========================\n";
    if (root){
        root->print(os);
    }
    else{
        os << "empty tree";
    }
}

template<class T>
std::pair<AVLTree<T>, AVLTree<T>> AVLTree<T>::split(AVLTree &tree, const T &value, bool left_is_strictly_Less) {
    auto nodes = split(tree.root, value, left_is_strictly_Less);
    return std::make_pair(AVLTree<T>(nodes.first), AVLTree<T>(nodes.second));
}


template<class T>
AVLTree<T>::AVLTree(const initializer_list<T> &il)
:root(sNode(nullptr)){
    for (auto el: il){
        insert(el);
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const AVLTree<T> &tree){
    tree.print(os);
    return os;
}

template <typename T>
bool operator==(const AVLTree<T> &tree, const std::set<T> &set){
    if (set.size() != tree.size()){
        return false;
    }

    auto tree_it = AVLIterator<T>(tree); // redo
    auto set_it = set.begin();
    for (; tree_it.currentNode() || set_it != set.end(); ++set_it, ++tree_it){
        if (*tree_it != *set_it){
            return false;
        }
    }
    return set_it == set.end() && tree_it.next() == nullptr;
}

template<class T>
AVLTree<T> setIntersection(const AVLTree<T> &first, const AVLTree<T> &second){
    AVLTree<T> intersection;
    AVLIterator<T> it1(first);
    AVLIterator<T> it2(second);
    while(it1.currentNode() && it2.currentNode()){
        T f_value = *it1;
        T s_value = *it2;
        if (f_value == s_value){
            intersection.insert(f_value);
            ++it1;
            ++it2;
        }
        else if(f_value < s_value){
            ++it1;
        }
        else{
            ++it2;
        }
    }

    return intersection;
}

template<class T>
AVLTree<T> setUnion(const AVLTree<T> &first, const AVLTree<T> &second){
    auto trees_union = AVLTree<T>(first);
    AVLIterator<T> it(second);
    while(it.currentNode()){
        trees_union.insert(*it);
        ++it;
    }

    return trees_union;
}

template<class T>
AVLTree<T> setDifference(const AVLTree<T> &first, const AVLTree<T> &second){
    auto trees_difference = AVLTree<T>(first);
    AVLIterator<T> it(second);
    while(it.currentNode()){
        trees_difference.deleteIfExists(*it);
        ++it;
    }
    return trees_difference;
}

template<class T>
bool operator ==(const AVLTree<T> &lhs, const AVLTree<T> &rhs){
    if (lhs.size() != rhs.size()){
        return false;
    }

    auto it1 = AVLIterator<T>(lhs);
    auto it2 = AVLIterator<T>(rhs);
    while(it1.currentNode() != nullptr){
        if (*it1 != *it2){
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

template<class T>
bool operator !=(const AVLTree<T> &lhs, const AVLTree<T> &rhs){
    return !(lhs == rhs);
}
