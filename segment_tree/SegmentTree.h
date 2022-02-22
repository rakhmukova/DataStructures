#pragma once

#include <vector>
#include <memory>
#include <queue>
#include <limits>
#include <cmath>

template<typename T>
class SegmentTree {
private:
    struct Node{
        T value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    public:
        explicit Node(const T &value, std::shared_ptr<Node> left = nullptr, std::shared_ptr<Node> right = nullptr);
    };

    std::vector<T> elements;
    std::shared_ptr<Node> root;
    T neutral_element = T();
    size_t pow_2;

    void set(size_t index, T value, std::shared_ptr<Node> &node, size_t left, size_t right);
    T sum(size_t left_border, size_t right_border, std::shared_ptr<Node> &node, size_t left, size_t right);
    void buildTree();

public:
    explicit SegmentTree(const std::vector<T> &elements);
    void set(size_t index, T value);
    T sum(size_t left_border, size_t right_border);
};



