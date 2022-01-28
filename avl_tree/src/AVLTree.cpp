#include "AVLTree.h"

using namespace std;
using namespace avl;

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::Node &AVLTree<T, TCompare>::Node::operator =(const typename AVLTree<T, TCompare>::Node &other){
    if (this != &other) {
        this->left_child = nullptr;
        this->right_child = nullptr; //instead of deletion
        auto *tmp = new typename AVLTree<T, TCompare>::Node(other);
        std::swap(*tmp, *this);
    }
    return *this;
}

template<typename T, typename TCompare>
void AVLTree<T, TCompare>::Node::updateHeight()
{
    this->subtree_height = 1 + std::max(left_child ? left_child->subtree_height : 0,
                                        right_child ? right_child->subtree_height : 0);
}

template<typename T, typename TCompare>
void AVLTree<T, TCompare>::Node::updateSize() {
    this->subtree_size = 1
                         + (left_child ? left_child->subtree_size : 0)
                         + (right_child ? right_child->subtree_size: 0);
}

template<typename T, typename TCompare>
AVLTree<T, TCompare>::Node::Node(const T &value, const sNode &left_child, const sNode &right_child)
        :value(value)
        ,left_child(left_child)
        ,right_child(right_child)
{
    updateHeight();
    updateSize();
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::Node::sNode AVLTree<T, TCompare>::Node::deepCopy() const{
    auto copy = std::make_shared<AVLTree<T, TCompare>::Node>(this->value);
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

template<typename T, typename TCompare>
AVLTree<T, TCompare>::Node::Node(const Node &other)
        :value(other.value)
{
    if (other.left_child){
        left_child = std::make_shared<AVLTree<T, TCompare>::Node>(AVLTree<T, TCompare>::Node(*other.left_child));
    }

    if (other.right_child){
        right_child = std::make_shared<AVLTree<T, TCompare>::Node>(AVLTree<T, TCompare>::Node(*other.right_child));
    }

    updateSize();
    updateHeight();
}

template<typename T, typename TCompare>
int AVLTree<T, TCompare>::Node::heightDiff() const {
    int left_height = left_child? left_child->getHeight() : 0;
    int right_height = right_child ? right_child->getHeight() : 0;
    return left_height - right_height;
}

template<typename T, typename TCompare>
void AVLTree<T, TCompare>::Node::print(ostream &os, uint indent) const{
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

template <typename T, typename TCompare>
std::ostream &operator<<(ostream &os, const typename AVLTree<T, TCompare>::Node &node) {
    node.print(os);
    return os;
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::sNode AVLTree<T, TCompare>::insert(sNode &subtree_root, const T &value) {
    if (!subtree_root){
        return make_shared<Node>(value);
    }

    auto cur_value = subtree_root->getValue();
    if (cmp(value, cur_value)){
        subtree_root->left_child = insert(subtree_root->left_child, value);
    }
    else if (cmp(cur_value, value)){
        subtree_root->right_child = insert(subtree_root->right_child, value);
    }
    else{
        return subtree_root;
    }

    subtree_root = recoverBalance(subtree_root);
    return subtree_root;
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::sNode AVLTree<T, TCompare>::findNode(const T &value, sNode subtree_root) {
    if (!subtree_root){
        return subtree_root;
    }

    auto cur_value = subtree_root->getValue();
    if (cmp(value, cur_value)){
        return findNode(value, subtree_root->left_child);
    }
    else if (cmp(cur_value, value)){
        return findNode(value, subtree_root->right_child);
    }
    else{
        return subtree_root;
    }
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::sNode AVLTree<T, TCompare>::deleteIfExists(const T &value, sNode &subtree_root) {
    if (!subtree_root){
        return nullptr;
    }

    auto cur_value = subtree_root->getValue();
    if(cmp(value, cur_value)){
        subtree_root->left_child = deleteIfExists(value, subtree_root->left_child);
    }
    else if(cmp(cur_value, value)){
        subtree_root->right_child = deleteIfExists(value, subtree_root->right_child);
    }
    else{
        if (!subtree_root->left_child){
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

    subtree_root = recoverBalance(subtree_root);
    return subtree_root;
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::sNode AVLTree<T, TCompare>::leftRotate(sNode k2) {
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

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::sNode AVLTree<T, TCompare>::rightRotate(sNode k2) {
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

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::sNode AVLTree<T, TCompare>::recoverBalance(sNode subtree_root) {
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

template<typename T, typename TCompare>
AVLTree<T, TCompare>::AVLTree(sNode root, const TCompare &cmp) //what's going on
        :root(root)
        ,cmp(cmp){}

template<typename T, typename TCompare>
AVLTree<T, TCompare>::AVLTree(const vector<T> &elements, const TCompare &cmp)
:root(nullptr)
,cmp(cmp){
    for (auto el : elements){
        insert(el);
    }
}

template<typename T, typename TCompare>
std::vector<T> AVLTree<T, TCompare>::toArray() const{
    vector<T> array;
    auto it = AVLIterator(*this); //copying is occuring
    while (it.currentNode() != nullptr){
        array.push_back(*it);
        ++it;
    }
    return array;
}

template<typename T, typename TCompare>
AVLTree<T, TCompare> &AVLTree<T, TCompare>::operator=(const AVLTree &other) {
    if (this != &other){
        AVLTree<T, TCompare>tmp(other);
        std::swap(*this, tmp);
    }
    return *this;
}

template<class T, class TCompare>
AVLTree<T, TCompare>::AVLTree(const AVLTree &other){
    if (other.root){
        root = make_shared<Node>(Node(*other.root));
    }
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::sNode
AVLTree<T, TCompare>::mergeWithRootAndBalance(AVLTree::sNode  left, AVLTree::sNode  right, AVLTree::sNode  subtree_root) {
    if (!subtree_root){
        return nullptr;
    }

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

template<typename T, typename TCompare>
pair<typename AVLTree<T, TCompare>::sNode  , typename AVLTree<T, TCompare>::sNode  >
AVLTree<T, TCompare>::split(AVLTree::sNode  subtree_root, const T &value, bool left_is_strictly_Less) {
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

template<typename T, typename TCompare>
void AVLTree<T, TCompare>::print(ostream &os) const {
    os << "tree:\n";
    if (root){
        root->print(os);
    }
    else{
        os << "tree is empty";
    }
}

template<typename T, typename TCompare>
std::pair<AVLTree<T, TCompare>, AVLTree<T, TCompare>> AVLTree<T, TCompare>::split(AVLTree &tree, const T &value, bool left_is_strictly_Less) {
    auto nodes = split(tree.root, value, left_is_strictly_Less);
    return std::make_pair(AVLTree<T, TCompare>(nodes.first), AVLTree<T, TCompare>(nodes.second));
}


template<typename T, typename TCompare>
AVLTree<T, TCompare>::AVLTree(const initializer_list<T> &il, const TCompare &cmp)
:root(sNode(nullptr))
,cmp(cmp){
    for (auto el: il){
        insert(el);
    }
}

template <typename T, typename TCompare>
std::ostream& operator<<(std::ostream& os, const AVLTree<T, TCompare> &tree){
    tree.print(os);
    return os;
}

template <typename T, typename TCompare>
bool operator==(const AVLTree<T, TCompare> &tree, const std::set<T, TCompare> &set){
    if (set.size() != tree.size()){
        return false;
    }

    auto tree_it = AVLIterator(tree); // redo
    auto set_it = set.begin();
    for (; tree_it.currentNode() || set_it != set.end(); ++set_it, ++tree_it){
        if (*tree_it != *set_it){
            return false;
        }
    }
    return set_it == set.end() && tree_it.next() == nullptr;
}

template<typename T, typename TCompare>
AVLTree<T, TCompare> setIntersection(const AVLTree<T, TCompare> &first, const AVLTree<T, TCompare> &second){
    AVLTree<T, TCompare> intersection;
    typename AVLTree<T, TCompare>::AVLIterator it1(first);
    typename AVLTree<T, TCompare>::AVLIterator it2(second);
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

template<typename T, typename TCompare>
AVLTree<T, TCompare> setUnion(const AVLTree<T, TCompare> &first, const AVLTree<T, TCompare> &second){
    auto trees_union = AVLTree<T, TCompare>(first);
    typename AVLTree<T, TCompare>::AVLIterator it(second);
    while(it.currentNode()){
        trees_union.insert(*it);
        ++it;
    }

    return trees_union;
}

template<typename T, typename TCompare>
AVLTree<T, TCompare> setDifference(const AVLTree<T, TCompare> &first, const AVLTree<T, TCompare> &second){
    auto trees_difference = AVLTree<T, TCompare>(first);
    typename AVLTree<T, TCompare>::AVLIterator it(second);
    while(it.currentNode()){
        trees_difference.deleteIfExists(*it);
        ++it;
    }
    return trees_difference;
}

template<typename T, typename TCompare>
bool operator ==(const AVLTree<T, TCompare> &lhs, const AVLTree<T, TCompare> &rhs){
    if (lhs.size() != rhs.size()){
        return false;
    }

    auto it1 = AVLIterator(lhs);
    auto it2 = AVLIterator(rhs);
    while(it1.currentNode() != nullptr){
        if (*it1 != *it2){
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

template<typename T, typename TCompare>
bool operator !=(const AVLTree<T, TCompare> &lhs, const AVLTree<T, TCompare> &rhs){
    return !(lhs == rhs);
}


template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::AVLIterator &AVLTree<T, TCompare>::AVLIterator::operator++(){
    if (current){
        pushLeftBranch(current->right_child);
    }

    return *this;
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::AVLIterator &AVLTree<T, TCompare>::AVLIterator::operator++(int){
    AVLIterator copy = *this;
    ++this;
    return copy;
}

template<typename T, typename TCompare>
AVLTree<T, TCompare>::AVLIterator::AVLIterator(const AVLTree<T>& tree){
    pushLeftBranch(tree.root);
}

template<typename T, typename TCompare>
typename AVLTree<T, TCompare>::AVLIterator &AVLTree<T, TCompare>::AVLIterator::operator=(const AVLTree<T, TCompare>::AVLIterator &other) {
    if (*this != other){
        this->nodes = other.nodes;
    }
    return *this;
}

template<typename T, typename TCompare>
void AVLTree<T, TCompare>::AVLIterator::pushLeftBranch(typename AVLTree<T, TCompare>::Node::sNode current_node) {
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