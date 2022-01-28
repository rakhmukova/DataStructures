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
void Node<T>::print(ostream &os, uint indent) const{ //may do that using inOrder
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

template<typename TKey>
Node<TKey> *AVLTree<TKey>::insert(Node<TKey>* subtree_root, const TKey &value) {
    if (subtree_root == nullptr){
        return new Node<TKey>(value);
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

template<class TKey>
Node<TKey> *AVLTree<TKey>::findNode(const TKey &value, Node<TKey> *subtree_root) {
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

template<typename TKey>
Node<TKey> *AVLTree<TKey>::deleteIfExists(const TKey &value, Node<TKey> *subtree_root) {
    if (subtree_root == nullptr){
        return nullptr;
    }

    if (value == subtree_root->getValue()){
        if (!subtree_root->left_child){
            TNode *temp = subtree_root;
            subtree_root = subtree_root->right_child;
            delete temp; //errors
            return subtree_root;
        }
        /*if (!subtree_root->right_child){
            subtree_root = subtree_root->left_child;
            return subtree_root;
        }*/
        //else - both children are presented
        Node<TKey> *temp = subtree_root->left_child;
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

template<class TKey>
Node<TKey> *AVLTree<TKey>::leftRotate(Node<TKey> *k2) {
    Node<TKey> *k1 = k2->right_child;
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

template<class TKey>
Node<TKey> *AVLTree<TKey>::rightRotate(Node<TKey> *k2) {
    Node<TKey> *k1 = k2->left_child;
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

template<class TKey>
Node<TKey> *AVLTree<TKey>::recoverBalance(Node<TKey> *subtree_root) {
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

template<class TKey>
AVLTree<TKey>::AVLTree(Node<TKey> *root)
        :root(root){}

template<class TKey>
AVLTree<TKey>::AVLTree(const vector<TKey> &elements)
:root(nullptr){
    for (auto el : elements){
        insert(el);
    }
}

template<class TKey>
std::vector<TKey> AVLTree<TKey>::toArray() const{
    vector<TKey> array;
    auto it = AVLIterator<TKey>(*this); //copying is occuring
    while (it.hasNext()){
        array.push_back(it.next());
    }
    return array;
}

template<class TKey>
AVLTree<TKey> &AVLTree<TKey>::operator=(const AVLTree &other) {
    if (this != &other){
        AVLTree<TKey>tmp(other);
        std::swap(*this, tmp);
    }
    return *this;
}

template<class TKey>
AVLTree<TKey>::AVLTree(const AVLTree &other){
    if (other.root){
        root = new Node<TKey>(*other.root);
    } //redo
}

template<class TKey>
AVLTree<TKey>::~AVLTree() {
    //delete root;
}

template<class TKey>
typename AVLTree<TKey>::TNode *
AVLTree<TKey>::mergeWithRootAndBalance(AVLTree::TNode *left, AVLTree::TNode *right, AVLTree::TNode *subtree_root) {
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

template<class TKey>
pair<typename AVLTree<TKey>::TNode *, typename AVLTree<TKey>::TNode *>
AVLTree<TKey>::split(AVLTree::TNode *subtree_root, const TKey &value, bool left_is_strictly_Less) {
    std::pair<TNode *, TNode *> result(nullptr, nullptr);
    if (!subtree_root){
        return result;
    }
    //go left
    if (value < subtree_root->getValue() || value == subtree_root->getValue() && left_is_strictly_Less){
        auto nodes = split(subtree_root->left_child, value, left_is_strictly_Less);
        auto new_right = mergeWithRootAndBalance(nodes.second, subtree_root->right_child, subtree_root);
        result.first = nodes.first;
        result.second = new_right;
        return result;
        //std::make_pair(nodes.first, new_right); //pass real values
    } //go right
    else{
        auto nodes = split(subtree_root->right_child, value, left_is_strictly_Less);
        auto new_left = mergeWithRootAndBalance(subtree_root->left_child, nodes.first, subtree_root);
        result.first = new_left;
        result.second = nodes.second;
        return result;
        //std::make_pair(new_left, nodes.second);
    }
}

template<class TKey>
void AVLTree<TKey>::print(ostream &os) const {
    os << "===========================\n";
    if (root){
        root->print(os);
    }
    else{
        os << "empty tree";
    }
}

template<class TKey>
AVLTree<TKey> &AVLTree<TKey>::remainWithinBorders(const TKey &left_border, const TKey &right_border) {
    auto left_pair = split(root, left_border, false);
    auto result_pair = split(left_pair.second, right_border, true);
    return result_pair.first;
}

template<class TKey>
AVLTree<TKey> &AVLTree<TKey>::removeMoreThan(const TKey &value) {
    auto pair = split(root, value, false);
    return pair.first;
}

template<class TKey>
AVLTree<TKey> &AVLTree<TKey>::removeLessThan(const TKey &value) {
    auto pair = split(root, value, true);
    return pair.second;
}

template<class TKey>
std::pair<AVLTree<TKey>, AVLTree<TKey>> AVLTree<TKey>::split(AVLTree &tree, const TKey &value, bool left_is_strictly_Less) {
    auto nodes = split(tree.root, value, left_is_strictly_Less);
    return std::make_pair(AVLTree<TKey>(nodes.first), AVLTree<TKey>(nodes.second));
}

template<class TKey>
AVLTree<TKey>::AVLTree()
:root(nullptr){}

template<class TKey>
AVLTree<TKey>::AVLTree(const initializer_list<TKey> &il)
:root(nullptr){
    for (auto el: il){
        insert(el);
    }
}

template <typename TKey>
std::ostream& operator<<(std::ostream& os, const AVLTree<TKey> &tree){
    tree.print(os);
    return os;
}

template <typename TKey>
bool operator==(const AVLTree<TKey> &tree, const std::set<TKey> &set){
    if (set.size() != tree.size()){
        return false;
    }

    auto tree_it = AVLIterator<TKey>(tree); // redo
    auto set_it = set.begin();
    for (; tree_it.next() != nullptr || set_it != set.end(); ++set_it, ++tree_it){
        if (*tree_it != *set_it){
            return false;
        }
    }
    return set_it == set.end() && tree_it.next() == nullptr;
}

template<class TKey>
AVLTree<TKey> setIntersection(const AVLTree<TKey> &first, const AVLTree<TKey> &second){
    AVLTree<TKey> intersection;
    AVLIterator<TKey> it1(first);
    AVLIterator<TKey> it2(second);
    while(it1.hasNext() && it2.hasNext()){
        TKey f_value = it1.next();
        TKey s_value = it2.next();
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

template<class TKey>
AVLTree<TKey> setUnion(const AVLTree<TKey> &first, const AVLTree<TKey> &second){
    auto trees_union = AVLTree<TKey>(first);
    AVLIterator<TKey> it(second);
    while(it.hasNext()){
        trees_union.insert(*it);
        ++it;
    }
    return trees_union;
}

template<class TKey>
AVLTree<TKey> setDifference(const AVLTree<TKey> &first, const AVLTree<TKey> &second){
    auto trees_difference = AVLTree<TKey>(first);
    AVLIterator<TKey> it(second);
    while(it.next()){
        trees_difference.deleteIfExists(*it);
        ++it;
    }
    return trees_difference;
}

template<class TKey>
bool operator ==(const AVLTree<TKey> &lhs, const AVLTree<TKey> &rhs){
    if (lhs.size() != rhs.size()){
        return false;
    }

    auto it1 = AVLIterator<TKey>(lhs);
    auto it2 = AVLIterator<TKey>(rhs);
    while(it1.hasNext()){
        if (*it1 != *it2){
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

template<class TKey>
bool operator !=(const AVLTree<TKey> &lhs, const AVLTree<TKey> &rhs){
    return !(lhs == rhs);
}
