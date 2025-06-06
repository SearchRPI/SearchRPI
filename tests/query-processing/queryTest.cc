#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <unordered_set>
#include "../include/query-processing/query.h"

class QueryTestFixture : public ::testing::Test {
protected:
    static std::unordered_set<std::string> dict;
    static bk::BKTree* bkTree;
    static queryTree::TermDictionary termDictionary;

    static void SetUpTestSuite() {
        // Define a small dictionary directly in the test
        dict = {"hello", "world", "run", "happy", "children", "toy", "apple",
                "banana", "good", "ski"};

        // Initialize BKTree with the small dictionary
        bkTree = new bk::BKTree(dict);

        // Define a small term dictionary
        termDictionary = {
            {"good ski", {}},
            {"ski resort", {}},
            {"fast runner", {}},
            {"mountain peak", {}}
        };
    }

    static void TearDownTestSuite() {
        delete bkTree;
    }
};

// Initialize static members
std::unordered_set<std::string> QueryTestFixture::dict;
bk::BKTree* QueryTestFixture::bkTree = nullptr;
queryTree::TermDictionary QueryTestFixture::termDictionary;

// Test that processQuery corrects spelling errors
TEST_F(QueryTestFixture, ProcessQueryReturnsCorrections) {
    std::string query = "Runnning happyness childrns toy";
    queryTree::QueryTree tree = query::processQuery(query, dict, *bkTree, termDictionary);

    ASSERT_FALSE(tree.getNode(0) == nullptr);
    EXPECT_EQ(tree.getNode(0)->getOperation(), queryTree::QueryOperator::COMBINE);
    ASSERT_EQ(tree.getNode(1)->getValue(), "runn");
    ASSERT_EQ(tree.getNode(2)->getValue(), "happy");
    ASSERT_EQ(tree.getNode(3)->getValue(), "children");
    ASSERT_EQ(tree.getNode(4)->getValue(), "toy");
}

// Test that processQuery handles punctuation and case correctly
TEST_F(QueryTestFixture, ProcessQueryHandlesPunctuationAndCase) {
    std::string query = "HeLLo, WoRLD!";
    queryTree::QueryTree tree = query::processQuery(query, dict, *bkTree, termDictionary);

    ASSERT_FALSE(tree.getNode(0) == nullptr);
    EXPECT_EQ(tree.getNode(0)->getOperation(), queryTree::QueryOperator::COMBINE);
    ASSERT_EQ(tree.getNode(1)->getValue(), "hello");
    ASSERT_EQ(tree.getNode(2)->getValue(), "world");
}

// Test that an empty query returns an empty result
TEST_F(QueryTestFixture, ProcessQueryHandlesEmptyInput) {
    std::string query = "";
    queryTree::QueryTree tree = query::processQuery(query, dict, *bkTree, termDictionary);
    EXPECT_EQ(tree.getNode(0), nullptr);
}

// Test that processQuery removes only stop words
TEST_F(QueryTestFixture, ProcessQueryRemovesOnlyStopWords) {
    std::string query = "the and in on at";
    queryTree::QueryTree tree = query::processQuery(query, dict, *bkTree, termDictionary);
    EXPECT_EQ(tree.getNode(0), nullptr);
}

// Test that processQuery handles Unicode input
TEST_F(QueryTestFixture, ProcessQueryHandlesUnicodeCharacters) {
    std::string query = "你好 世界"; // "Hello World" in Chinese
    queryTree::QueryTree tree = query::processQuery(query, dict, *bkTree, termDictionary);
    ASSERT_FALSE(tree.getNode(0) == nullptr);
}

// Test that words without typos remain unchanged and phrases are detected
TEST_F(QueryTestFixture, ProcessQueryBuildsQueryTreeForPhrases) {
    std::string query = "What are good skis?";
    queryTree::QueryTree tree = query::processQuery(query, dict, *bkTree, termDictionary);

    ASSERT_FALSE(tree.getNode(0) == nullptr);
    EXPECT_EQ(tree.getNode(0)->getOperation(), queryTree::QueryOperator::COMBINE);
    ASSERT_EQ(tree.getNode(1)->getOperation(), queryTree::QueryOperator::OD);  // "good skis" should be grouped

    // Check that "good skis" is inside an `#od:1` node
    EXPECT_EQ(tree.getNode(2)->getOperation(), queryTree::QueryOperator::TEXT);
    EXPECT_EQ(tree.getNode(2)->getValue(), "good");
    EXPECT_EQ(tree.getNode(3)->getOperation(), queryTree::QueryOperator::TEXT);
    EXPECT_EQ(tree.getNode(3)->getValue(), "ski");
}