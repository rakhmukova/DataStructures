cmake_minimum_required(VERSION 3.20)
project(mylib VERSION 1.0.1 DESCRIPTION "mylib description")

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(GoogleTest)
add_library(mylib Node.cpp AVLTree.cpp AVLIterator.cpp AVLIterator.h)
