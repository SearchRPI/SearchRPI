#include "../include/query-processing/queryTree.h"
#include <gtest/gtest.h>
#include <sstream>

class QueryTreeTest : public ::testing::Test {
protected:
    queryTree::TermDictionary termDictionary;
    queryTree::TokenList tokens;

    void SetUp() override {
        // Define a small term dictionary (manual phrases)
        termDictionary = {
            {"good skis", {}},  // Ensures "good skis" is detected as a phrase
        };

        // Sample processed query tokens (unaltered)
        tokens = {"good", "skis"};
    }
};

// Test QueryNode constructor with only an operation
TEST_F(QueryTreeTest, QueryNodeOperationConstructor) {
    queryTree::QueryNode node("#combine");
    EXPECT_EQ(node.operation, "#combine");
    EXPECT_TRUE(node.children.empty());
    EXPECT_TRUE(node.value.empty());
}

// Test QueryNode constructor with an operation and value
TEST_F(QueryTreeTest, QueryNodeOperationValueConstructor) {
    queryTree::QueryNode node("#text", "good");
    EXPECT_EQ(node.operation, "#text");
    EXPECT_EQ(node.value, "good");
    EXPECT_TRUE(node.children.empty());
}

// Test building a query tree with phrase and term weighting
TEST_F(QueryTreeTest, BuildQueryTreeBasic) {
    auto root = queryTree::buildQueryTree(tokens, termDictionary);
    ASSERT_NE(root, nullptr);

    // Assertions
    EXPECT_EQ(root->operation, "#combine");
    EXPECT_EQ(root->children.size(), 1);  // "good skis" should be merged into one phrase

    // Check the first child (should be an ordered phrase `#od:1`)
    auto firstChild = root->children[0];
    EXPECT_EQ(firstChild->operation, "#od:1");
    EXPECT_EQ(firstChild->children.size(), 2);
    EXPECT_EQ(firstChild->children[0]->operation, "#text");
    EXPECT_EQ(firstChild->children[0]->value, "good");
    EXPECT_EQ(firstChild->children[1]->operation, "#text");
    EXPECT_EQ(firstChild->children[1]->value, "skis");
}

// Test printing the query tree
TEST_F(QueryTreeTest, PrintQueryTreeOutput) {
    auto root = queryTree::buildQueryTree(tokens, termDictionary);
    ASSERT_NE(root, nullptr);

    // Redirect cout to a string stream
    std::ostringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());

    // Print tree
    queryTree::printQueryTree(root);

    // Restore cout
    std::cout.rdbuf(oldCout);

    // Verify output structure (approximate)
    std::string printedTree = output.str();
    EXPECT_NE(printedTree.find("#combine"), std::string::npos);
    EXPECT_NE(printedTree.find("#od:1"), std::string::npos);
    EXPECT_NE(printedTree.find("#text (good)"), std::string::npos);
    EXPECT_NE(printedTree.find("#text (skis)"), std::string::npos);
}
