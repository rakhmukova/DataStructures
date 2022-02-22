#include "SegmentTree.h"

template<typename T>
SegmentTree<T>::SegmentTree(const std::vector<T> &elements)
        :elements(elements)
        ,pow_2(pow(2, ceil(log(elements.size())/log(2)))){
    buildTree();
}

template<typename T>
void SegmentTree<T>::buildTree() {
    std::queue<std::shared_ptr<Node>> layer;
    for (auto el : elements){
        layer.push(std::shared_ptr<Node>(new Node(el)));
    }

    while (layer.size() != 1){
        size_t cur_size = layer.size();
        for (size_t i = 0; i < cur_size; i += 2){
            auto left = layer.front();
            layer.pop();
            std::shared_ptr<Node> right = nullptr;
            if (i < cur_size - 1 || cur_size % 2 == 0){
                right = layer.front();
                layer.pop();
            }
            auto parent = std::shared_ptr<Node>(new Node(left->value + (right? right->value : 0), left, right));
            layer.push(parent);
        }
    }

    root = layer.front();
    layer.pop();
}

template<typename T>
void SegmentTree<T>::set(size_t index, T value, std::shared_ptr<Node> &node, size_t left, size_t right) {
    if (!node){
        return;
    }

    if (right == left){ //redo
        node->value = value;
        return;
    }

    size_t m = (left + right) / 2;
    if (index <= m){ //ends are included
        set(index, value, node->left, left, m);
    } else{
        set(index, value, node->right, m + 1, right);
    }

    node->value =
            (node->left? node->left->value : 0)
            +
            (node->right? node->right->value : 0);
}

template<typename T>
T SegmentTree<T>::sum(size_t left_border, size_t right_border, std::shared_ptr<Node> &node, size_t left, size_t right) {
    if (left > right_border || right < left_border){
        return neutral_element;
    }

    if (left >= left_border && right <= right_border){
        return node->value;
    }

    size_t m = (left + right) / 2;
    return sum(left_border, right_border, node->left, left, m)
           + sum(left_border, right_border, node->right, m + 1, right);
}

template<typename T>
T SegmentTree<T>::sum(size_t left_border, size_t right_border) {
    return sum(left_border, right_border, root, 0, pow_2 - 1);
}

template<typename T>
void SegmentTree<T>::set(size_t index, T value) {
    set(index, value, root, 0, pow_2 - 1);
}

template<typename T>
SegmentTree<T>::Node::Node(const T &value, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        :value(value)
        ,left(left)
        ,right(right){}
