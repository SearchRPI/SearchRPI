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

// Test building a query tree with phrase and term weighting
TEST_F(QueryTreeTest, BuildQueryTreeBasic) {
    queryTree::QueryTree tree(tokens, termDictionary);

    // Check root node
    const auto* root = tree.getNode(0);
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getOperation(), queryTree::QueryOperator::COMBINE);
    EXPECT_EQ(root->getChildCount(), 1);  // "good skis" phrase node

    // Check first child (phrase node)
    const auto* phraseNode = tree.getNode(root->getChildStart());
    ASSERT_NE(phraseNode, nullptr);
    EXPECT_EQ(phraseNode->getOperation(), queryTree::QueryOperator::OD);
    EXPECT_EQ(phraseNode->getChildCount(), 2);

    // Check individual term nodes
    const auto* firstTerm = tree.getNode(phraseNode->getChildStart());
    ASSERT_NE(firstTerm, nullptr);
    EXPECT_EQ(firstTerm->getOperation(), queryTree::QueryOperator::TEXT);
    EXPECT_EQ(firstTerm->getValue(), "good");

    const auto* secondTerm = tree.getNode(phraseNode->getChildStart() + 1);
    ASSERT_NE(secondTerm, nullptr);
    EXPECT_EQ(secondTerm->getOperation(), queryTree::QueryOperator::TEXT);
    EXPECT_EQ(secondTerm->getValue(), "skis");
}

TEST_F(QueryTreeTest, TraverseQueryTreeWithDepthHelper) {
    queryTree::QueryTree tree(tokens, termDictionary);
    std::ostringstream oss;

    tree.forEachNodeWithDepth([&](const queryTree::QueryNode& node, int depth) {
        for (int i = 0; i < depth; ++i)
            oss << "  ";

        switch (node.getOperation()) {
            case queryTree::QueryOperator::COMBINE:
                oss << "#combine\n";
                break;
            case queryTree::QueryOperator::OD:
                oss << "#od\n";
                break;
            case queryTree::QueryOperator::TEXT:
                oss << "#text:" << node.getValue() << "\n";
                break;
            default:
                oss << "#unknown\n";
                break;
        }
    });

    std::string expected =
        "#combine\n"
        "  #od\n"
        "    #text:good\n"
        "    #text:skis\n";

    EXPECT_EQ(oss.str(), expected);
}


// Test printing the query tree
TEST_F(QueryTreeTest, PrintsQueryTreeStructure) {
    queryTree::QueryTree tree(tokens, termDictionary);

    std::ostringstream oss;
    oss << tree;

    std::string expected =
        "#combine\n"
        "  #od\n"
        "    #text:good\n"
        "    #text:skis\n";

    EXPECT_EQ(oss.str(), expected);
}
