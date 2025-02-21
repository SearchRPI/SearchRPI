#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <unordered_set>
#include "../include/query-processing/query.h"

class QueryTestFixture : public ::testing::Test {
protected:
    static std::unordered_set<std::string> dict;
    static bk::BKTree* tree;
    static queryTree::TermDictionary termDictionary;
    static queryTree::TermDictionary thesaurus;

    static void SetUpTestSuite() {
        // Define a small dictionary directly in the test
        dict = {"hello", "world", "run", "happy", "children", "toy", "apple",
                "banana"};

        // Initialize BKTree with the small dictionary
        tree = new bk::BKTree(dict);

        // Define a small term dictionary
        termDictionary = {
            {"good ski", {"high-quality skis", "premium skis"}},
            {"mountain", {"hill", "peak", "summit"}},
            {"fast runner", {"sprinter", "quick athlete"}},
            {"ski resort", {"snowy getaway", "winter vacation"}}
        };

        // Define a small thesaurus
        thesaurus = {
            {"good", {"great", "excellent", "decent", "best"}},
            {"ski", {"ski gear", "snowboard", "winter sports"}},
            {"fast", {"quick", "speedy", "rapid"}},
            {"mountain", {"hill", "peak", "summit"}},
            {"runner", {"athlete", "sprinter", "marathoner"}}
        };
    }

    static void TearDownTestSuite() {
        delete tree;
    }
};

// Initialize static members
std::unordered_set<std::string> QueryTestFixture::dict;
bk::BKTree* QueryTestFixture::tree = nullptr;
queryTree::TermDictionary QueryTestFixture::termDictionary;
queryTree::TermDictionary QueryTestFixture::thesaurus;

// Test that processQuery corrects spelling errors
TEST_F(QueryTestFixture, ProcessQueryReturnsCorrections) {
    std::string query = "Runnning happyness childrns toy";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary, thesaurus
    );

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->operation, "#combine");
    ASSERT_EQ(root->children.size(), 4);
    EXPECT_EQ(root->children[0]->value, "runn");
    EXPECT_EQ(root->children[1]->value, "happy");
    EXPECT_EQ(root->children[2]->value, "children");
    EXPECT_EQ(root->children[3]->value, "toy");
}

// Test that processQuery handles punctuation and case correctly
TEST_F(QueryTestFixture, ProcessQueryHandlesPunctuationAndCase) {
    std::string query = "HeLLo, WoRLD!";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary, thesaurus
    );

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->operation, "#combine");
    ASSERT_EQ(root->children.size(), 2);
    EXPECT_EQ(root->children[0]->value, "hello");
    EXPECT_EQ(root->children[1]->value, "world");
}

// Test that an empty query returns an empty result
TEST_F(QueryTestFixture, ProcessQueryHandlesEmptyInput) {
    std::string query = "";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary, thesaurus
    );
    EXPECT_EQ(root, nullptr);
}

TEST_F(QueryTestFixture, ProcessQueryRemovesOnlyStopWords) {
    std::string query = "the and in on at";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary, thesaurus
    );
    EXPECT_EQ(root, nullptr);
}

// Test that processQuery handles Unicode input
TEST_F(QueryTestFixture, ProcessQueryHandlesUnicodeCharacters) {
    std::string query = "你好 世界"; // "Hello World" in Chinese
    auto root = query::processQuery(
        query, dict, *tree, termDictionary, thesaurus);
    ASSERT_NE(root, nullptr);
}

// Test that words without typos remain unchanged
TEST_F(QueryTestFixture, ProcessQueryBuildsQueryTreeForGoodSkis) {
    std::string query = "What are good skis?";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary, thesaurus
    );

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->operation, "#combine");
    ASSERT_EQ(root->children.size(), 2);

    // Check that "good" is in a #syn node
    auto firstChild = root->children[0];
    EXPECT_EQ(firstChild->operation, "#syn");
    ASSERT_EQ(firstChild->children.size(), 5);

    // Check that "skis" is in a #syn node
    auto secondChild = root->children[1];
    EXPECT_EQ(secondChild->operation, "#syn");
    ASSERT_EQ(secondChild->children.size(), 4);
}

// Fake test
/*TEST_F(QueryTestFixture, PrintQueryTreeForGoodSkis) {*/
/*    std::string query = "What are good skis?";*/
/*    auto root = query::processQuery(*/
/*        query, dict, *tree, termDictionary, thesaurus*/
/*    );*/
/**/
/*    ASSERT_NE(root, nullptr);*/
/**/
/*    std::cout << "\n=== Query Tree for: \"" << query << "\" ===\n";*/
/*    queryTree::printQueryTree(root);*/
/*    std::cout << "======================================" << std::endl;*/
/*}*/
