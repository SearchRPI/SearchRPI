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

    static void SetUpTestSuite() {
        // Define a small dictionary directly in the test
        dict = {"hello", "world", "run", "happy", "children", "toy", "apple",
                "banana", "good", "ski"};

        // Initialize BKTree with the small dictionary
        tree = new bk::BKTree(dict);

        // Define a small term dictionary
        termDictionary = {
            {"good ski", {}},
            {"ski resort", {}},
            {"fast runner", {}},
            {"mountain peak", {}}
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

// Test that processQuery corrects spelling errors
TEST_F(QueryTestFixture, ProcessQueryReturnsCorrections) {
    std::string query = "Runnning happyness childrns toy";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary
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
        query, dict, *tree, termDictionary
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
        query, dict, *tree, termDictionary
    );
    EXPECT_EQ(root, nullptr);
}

// Test that processQuery removes only stop words
TEST_F(QueryTestFixture, ProcessQueryRemovesOnlyStopWords) {
    std::string query = "the and in on at";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary
    );
    EXPECT_EQ(root, nullptr);
}

// Test that processQuery handles Unicode input
TEST_F(QueryTestFixture, ProcessQueryHandlesUnicodeCharacters) {
    std::string query = "你好 世界"; // "Hello World" in Chinese
    auto root = query::processQuery(
        query, dict, *tree, termDictionary);
    ASSERT_NE(root, nullptr);
}

// Test that words without typos remain unchanged and phrases are detected
TEST_F(QueryTestFixture, ProcessQueryBuildsQueryTreeForGoodSkis) {
    std::string query = "What are good skis?";
    auto root = query::processQuery(
        query, dict, *tree, termDictionary
    );

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->operation, "#combine");
    ASSERT_EQ(root->children.size(), 1);  // "good skis" should be grouped

    // Check that "good skis" is inside an `#od:1` node
    auto firstChild = root->children[0];
    EXPECT_EQ(firstChild->operation, "#od:1");
    ASSERT_EQ(firstChild->children.size(), 2);
    EXPECT_EQ(firstChild->children[0]->operation, "#text");
    EXPECT_EQ(firstChild->children[0]->value, "good");
    EXPECT_EQ(firstChild->children[1]->operation, "#text");
    EXPECT_EQ(firstChild->children[1]->value, "ski");
}

// Fake test
TEST_F(QueryTestFixture, FakeTest) {
    std::string query = "What are good skis?";
    std::string query1 = "fast runner";  // Simple 2-word query
    std::string query2 = "hello world ski";  // 3 words, mix of common terms
    std::string query3 = "good skis in a ski resort";  // 6 words, contains a phrase
    std::string query4 = "What is the best mountain peak for skiing?";  // 10 words, natural query
    std::string query5 = "Looking for the highest rated ski resort with good snow and easy trails";  // 15 words, long and detailed
    auto root = query::processQuery(
        query, dict, *tree, termDictionary
    );

    ASSERT_NE(root, nullptr);

    std::cout << "\n=== Query Tree for: \"" << query << "\" ===\n";
    queryTree::printQueryTree(root);
    std::cout << "======================================" << std::endl;

    root = query::processQuery(
        query1, dict, *tree, termDictionary
    );

    ASSERT_NE(root, nullptr);

    std::cout << "\n=== Query Tree for: \"" << query1 << "\" ===\n";
    queryTree::printQueryTree(root);
    std::cout << "======================================" << std::endl;

    root = query::processQuery(
        query2, dict, *tree, termDictionary
    );

    ASSERT_NE(root, nullptr);

    std::cout << "\n=== Query Tree for: \"" << query2 << "\" ===\n";
    queryTree::printQueryTree(root);
    std::cout << "======================================" << std::endl;

    root = query::processQuery(
        query3, dict, *tree, termDictionary
    );

    ASSERT_NE(root, nullptr);

    std::cout << "\n=== Query Tree for: \"" << query3 << "\" ===\n";
    queryTree::printQueryTree(root);
    std::cout << "======================================" << std::endl;
    
    root = query::processQuery(
        query4, dict, *tree, termDictionary
    );

    ASSERT_NE(root, nullptr);

    std::cout << "\n=== Query Tree for: \"" << query4 << "\" ===\n";
    queryTree::printQueryTree(root);
    std::cout << "======================================" << std::endl;
    
    root = query::processQuery(
        query5, dict, *tree, termDictionary
    );

    ASSERT_NE(root, nullptr);

    std::cout << "\n=== Query Tree for: \"" << query5 << "\" ===\n";
    queryTree::printQueryTree(root);
    std::cout << "======================================" << std::endl;
}
