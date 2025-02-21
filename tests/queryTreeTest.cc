#include "../include/query-processing/queryTree.h"
#include <gtest/gtest.h>
#include <sstream>

class QueryTreeTest : public ::testing::Test {
protected:
    queryTree::TermDictionary termDictionary;
    queryTree::TermDictionary thesaurus;
    queryTree::TokenList tokens;

    void SetUp() override {
        // Define a small term dictionary (manual phrases)
        termDictionary = {
            {"good skis", {"high-quality skis", "premium skis"}},
            {"mountain", {"hill", "peak", "summit"}}
        };

        // Define a small thesaurus (manual synonyms)
        thesaurus = {
            {"good", {"great", "excellent", "decent"}},
            {"skis", {"ski gear", "snowboard"}},
            {"fast", {"quick", "speedy", "rapid"}}
        };

        // Sample processed query tokens
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
    queryTree::QueryNode node("#syn", "good");
    EXPECT_EQ(node.operation, "#syn");
    EXPECT_EQ(node.value, "good");
    EXPECT_TRUE(node.children.empty());
}

// Test building a query tree with simple tokens
TEST_F(QueryTreeTest, BuildQueryTreeBasic) {
    auto root = queryTree::buildQueryTree(tokens, termDictionary, thesaurus);
    ASSERT_NE(root, nullptr);

    EXPECT_EQ(root->operation, "#combine");
    EXPECT_EQ(root->children.size(), 2);

    // Check the first child (good)
    auto firstChild = root->children[0];
    EXPECT_EQ(firstChild->operation, "#syn");
    EXPECT_EQ(firstChild->children.size(), 4);

    // Check the second child (skis)
    auto secondChild = root->children[1];
    EXPECT_EQ(secondChild->operation, "#syn");
    EXPECT_EQ(secondChild->children.size(), 3);
}

/*// Test printing the query tree (redirects output for verification)*/
TEST_F(QueryTreeTest, PrintQueryTreeOutput) {
    auto root = queryTree::buildQueryTree(tokens, termDictionary, thesaurus);
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
    EXPECT_NE(printedTree.find("#syn"), std::string::npos);
    EXPECT_NE(printedTree.find("#syn"), std::string::npos);
}
