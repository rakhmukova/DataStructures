#include "gtest/gtest.h"
#include "AVLTree.h"
#include "AVLTree.cpp"
#include <algorithm>

using namespace std;
using namespace avl;

TEST(TreeOperations, InsertAndDelete){
    AVLTree<int> tree;
    set<int> set;
    size_t size = 78888; //20000000 - 35 sec
    vector<int> vector(size, 0);
    srand(time(nullptr));
    int modulus = 200000;
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    for (auto el: vector){
        tree.insert(el);
        set.insert(el);
        ASSERT_TRUE(tree.contains(el));
        ASSERT_EQ(tree.size(), set.size());
    }

    for (auto el: vector){
        tree.deleteIfExists(el);
        set.erase(el);

        //EXPECT_FALSE(tree.contains(el));
        //ASSERT_EQ(set.find(el), set.end());
        //EXPECT_EQ(tree.size(), set.size());
    }
}

TEST(TreeOperations, ToArray){
    size_t size = 10000;
    vector<int> vector(size, 0);
    srand(time(nullptr));
    int modulus = 2000;
    generate(vector.begin(), vector.end(), [modulus] () { return random() % modulus; });
    AVLTree<int> tree(vector);

    auto vector_copy = tree.toArray();
    std::sort(vector.begin(), vector.end());
    vector.erase(unique(vector.begin(), vector.end()), vector.end());

    ASSERT_EQ(vector, vector_copy);
}

TEST(TreeOperations, SplitByValueOutOfBorders){
    size_t size = 100;
    vector<int> vector(size, 0);
    int modulus = 2000;
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    AVLTree<int> tree(vector);
    AVLTree<int> copy(tree);
    auto splited = AVLTree<int>::split(tree, modulus, true);
    //ASSERT_EQ(splited.first, copy);
}

TEST(TreeOperations, SplitByValueWithinBorders){
    size_t size = 100;
    vector<int> vector(size, 0);
    int modulus = 2000;
    int index;
    generate(vector.begin(), vector.end(), [modulus, &index] () {
        long temp = random() % modulus;
        if (index % 2 == 0){
            temp += modulus;
        }
        ++index;
        return temp;
    });
    AVLTree<int> tree(vector);
    tree.print(cout);
    AVLTree<int> copy(tree);
    auto splited = AVLTree<int>::split(tree, modulus, true);
}

TEST(SetOperations, EmptyIntersection){
    size_t size = 200;
    vector<int> vector(size, 0);
    int modulus = 2000;
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    AVLTree<int> f_tree(vector);
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus + modulus; });
    AVLTree<int> s_tree(vector);
    auto intersection = ::setIntersection(f_tree, s_tree);
    ASSERT_EQ(0, intersection.size());
}

TEST(SetOperations, IntersectionOfTheSameSets){
    size_t size = 20;
    vector<int> vector(size, 0);
    int modulus = 2000;
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    AVLTree<int> f_tree(vector);
    AVLTree<int> s_tree(vector);
    auto intersection = ::setIntersection(f_tree, s_tree);
    ASSERT_EQ(intersection.size(), f_tree.size());
}

TEST(SetOperations, IntersectionIfOneSetIsEmty){
    size_t size = 20;
    vector<int> vector(size, 0);
    int modulus = 2000;
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    AVLTree<int> f_tree(vector);
    AVLTree<int> s_tree;
    auto intersection = ::setIntersection(f_tree, s_tree);
    ASSERT_EQ(0, intersection.size());
}

TEST(SetOperations, EmptyIntersectionOfOverlappingTrees){
    AVLTree<int> f_tree = {1, 4, 5, 8, 2};
    AVLTree<int> s_tree = {3, 7, 6, 12, 13};
    auto intersection = ::setIntersection(f_tree, s_tree);
    intersection.print(cout);
}

TEST(SetOperations, UnionOfNonOverlappingSets){
    size_t size = 200;
    vector<int> vector(size, 0);
    int modulus = 2000;
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    AVLTree<int> f_tree(vector);
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus + modulus; });
    AVLTree<int> s_tree(vector);
    auto union_trees = ::setUnion(f_tree, s_tree);
    ASSERT_EQ(f_tree.size() + s_tree.size(), union_trees.size());
}

TEST(SetOperations, UnionAndIntersectionSizeRelation){
    AVLTree<int> f_tree = {1, 5, 7, 8, 2};
    AVLTree<int> s_tree = {3, 7, 6, 12, 13};
    auto intersection = ::setIntersection(f_tree, s_tree);
    auto union_trees = ::setUnion(f_tree, s_tree);
    ASSERT_EQ(f_tree.size() + s_tree.size() - intersection.size() , union_trees.size());
    union_trees.print(cout);
}

TEST(SetOperations, Difference){
    size_t size = 200;
    vector<int> vector(size, 0);
    int modulus = 50;
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    AVLTree<int> f_tree(vector);
    generate(vector.begin(), vector.end(), [modulus] () {return random() % modulus; });
    AVLTree<int> s_tree(vector);
    auto difference = ::setDifference(f_tree, s_tree);
    auto intersection = ::setIntersection(s_tree, difference);
    ASSERT_EQ(0, intersection.size());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}