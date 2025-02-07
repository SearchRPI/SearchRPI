#include "../include/query-processing/bkTree.h"
#include <gtest/gtest.h>
#include <unordered_set>

class BKTreeTest : public ::testing::Test {
protected:
    bk::BKTree tree;
    std::unordered_set<std::string> dictionary = {"apple", "banana", "orange", "grape", "pineapple"};

    void SetUp() override {
        tree = bk::BKTree(dictionary);
    }
};

// Test default constructor (empty tree)
TEST_F(BKTreeTest, DefaultConstructorCreatesEmptyTree) {
    bk::BKTree emptyTree;
    EXPECT_EQ(emptyTree.findClosest("apple", 2), "apple");
}

// Test constructor that initializes from a dictionary
TEST_F(BKTreeTest, ConstructorInitializesWithDictionary) {
    EXPECT_EQ(tree.findClosest("appl", 2), "apple");
    EXPECT_EQ(tree.findClosest("banan", 2), "banana");
}

// Test insertion of a new word into the tree
TEST_F(BKTreeTest, InsertNewWordIntoTree) {
    tree.insert("strawberry");
    EXPECT_EQ(tree.findClosest("stawberry", 2), "strawberry");
}

// Test finding closest match with various distances
TEST_F(BKTreeTest, FindClosestWordWithThreshold) {
    EXPECT_EQ(tree.findClosest("appl", 2), "apple");    // "appl" -> "apple"
    EXPECT_EQ(tree.findClosest("banan", 2), "banana");  // "banan" -> "banana"
    EXPECT_EQ(tree.findClosest("orang", 2), "orange");  // "orang" -> "orange"
}

// Test edge cases with no close match
TEST_F(BKTreeTest, FindClosestNoMatch) {
    EXPECT_EQ(tree.findClosest("xyz", 1), "xyz");
}

// Test finding a word exactly as it exists in the tree
TEST_F(BKTreeTest, FindExactMatch) {
    EXPECT_EQ(tree.findClosest("apple", 1), "apple");
    EXPECT_EQ(tree.findClosest("banana", 1), "banana");
}

// Test behavior with empty input
TEST_F(BKTreeTest, FindClosestEmptyInput) {
    EXPECT_EQ(tree.findClosest("", 2), "");
}

// Test behavior with a large threshold
TEST_F(BKTreeTest, FindClosestLargeThreshold) {
    EXPECT_EQ(tree.findClosest("apxple", 5), "apple");
}
