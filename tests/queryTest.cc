#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <unordered_set>
#include "../include/query-processing/query.h"

class QueryTestFixture : public ::testing::Test {
protected:
    static std::unordered_set<std::string> dict;
    static bk::BKTree* tree;

    static void SetUpTestSuite() {
        // Define a small dictionary directly in the test
        dict = {"hello", "world", "run", "happy", "children", "toy", "apple",
                "banana"};

        // Initialize BKTree with the small dictionary
        tree = new bk::BKTree(dict);
    }

    static void TearDownTestSuite() {
        delete tree;
    }
};

// Initialize static members
std::unordered_set<std::string> QueryTestFixture::dict;
bk::BKTree* QueryTestFixture::tree = nullptr;

// Test that BKTree root is valid
TEST_F(QueryTestFixture, GetRootReturnsValidNode) {
    const auto* root = tree->getRoot();
    ASSERT_NE(root, nullptr);
    EXPECT_FALSE(root->word.empty());
    EXPECT_TRUE(dict.find(root->word) != dict.end());
}

// Test that processQuery corrects spelling errors
TEST_F(QueryTestFixture, ProcessQueryReturnsCorrections) {
    std::string query = "Runnning happyness childrns toy";
    auto tokens = query::processQuery(query, dict, *tree);
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0], "runn");
    EXPECT_EQ(tokens[1], "happy");
    EXPECT_EQ(tokens[2], "children");
    EXPECT_EQ(tokens[3], "toy");
}

// Test that processQuery handles punctuation and case correctly
TEST_F(QueryTestFixture, ProcessQueryHandlesPunctuationAndCase) {
    std::string query = "HeLLo, WoRLD!";
    auto tokens = query::processQuery(query, dict, *tree);
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "hello");
    EXPECT_EQ(tokens[1], "world");
}

// Test that an empty query returns an empty result
TEST_F(QueryTestFixture, ProcessQueryHandlesEmptyInput) {
    std::string query = "";
    auto tokens = query::processQuery(query, dict, *tree);
    EXPECT_TRUE(tokens.empty());
}

// Test that stop words are removed correctly
TEST_F(QueryTestFixture, ProcessQueryRemovesOnlyStopWords) {
    std::string query = "the and in on at";
    auto tokens = query::processQuery(query, dict, *tree);
    EXPECT_TRUE(tokens.empty()); // Assuming stop words are removed
}

// Test that processQuery handles Unicode input
TEST_F(QueryTestFixture, ProcessQueryHandlesUnicodeCharacters) {
    std::string query = "你好 世界"; // "Hello World" in Chinese
    auto tokens = query::processQuery(query, dict, *tree);
    ASSERT_FALSE(tokens.empty()); // Expecting some processing
}

// Test that words without typos remain unchanged
TEST_F(QueryTestFixture, ProcessQueryNoCorrectionsNeeded) {
    std::string query = "perfectli correct sentenc";
    auto tokens = query::processQuery(query, dict, *tree);
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "perfectli");
    EXPECT_EQ(tokens[1], "correct");
    EXPECT_EQ(tokens[2], "sentenc");
}
