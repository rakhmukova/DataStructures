cmake_minimum_required(VERSION 3.20)
project(avl_tree_lib)

set(CMAKE_CXX_STANDARD 14)

add_library(avl_tree_lib STATIC AVLTree.cpp)