#include <gtest/gtest.h>
#include "../../include/query-processing/queryNode.h"

using namespace queryTree;

class QueryNodeTest : public ::testing::Test {
protected:
    // Common test data
    QueryNode operationNode{0, QueryOperator::OD, "", 1, 2};
    QueryNode valueNode{1, QueryOperator::TEXT, "skis", -1, 0};
};

TEST_F(QueryNodeTest, ConstructorInitialization) {
    EXPECT_EQ(operationNode.getNodeIndex(), 0);
    EXPECT_EQ(operationNode.getOperation(), QueryOperator::OD);
    EXPECT_EQ(operationNode.getValue(), "");
    EXPECT_EQ(operationNode.getChildStart(), 1);
    EXPECT_EQ(operationNode.getChildCount(), 2);

    EXPECT_EQ(valueNode.getNodeIndex(), 1);
    EXPECT_EQ(valueNode.getOperation(), QueryOperator::TEXT);
    EXPECT_EQ(valueNode.getValue(), "skis");
    EXPECT_EQ(valueNode.getChildStart(), -1);
    EXPECT_EQ(valueNode.getChildCount(), 0);
}

TEST_F(QueryNodeTest, IsOperationCheck) {
    EXPECT_TRUE(operationNode.isOperation());
    EXPECT_FALSE(valueNode.isOperation());
}

TEST_F(QueryNodeTest, GetOperation) {
    EXPECT_EQ(operationNode.getOperation(), QueryOperator::OD);
    EXPECT_EQ(valueNode.getOperation(), QueryOperator::TEXT);
}

TEST_F(QueryNodeTest, GetValue) {
    EXPECT_EQ(operationNode.getValue(), "");
    EXPECT_EQ(valueNode.getValue(), "skis");
}

TEST_F(QueryNodeTest, GetChildStartAndCount) {
    EXPECT_EQ(operationNode.getChildStart(), 1);
    EXPECT_EQ(operationNode.getChildCount(), 2);

    EXPECT_EQ(valueNode.getChildStart(), -1);
    EXPECT_EQ(valueNode.getChildCount(), 0);
}

TEST_F(QueryNodeTest, GetNodeIndex) {
    EXPECT_EQ(operationNode.getNodeIndex(), 0);
    EXPECT_EQ(valueNode.getNodeIndex(), 1);
}

TEST_F(QueryNodeTest, SetChildStart) {
    QueryNode testNode{3, QueryOperator::UW, "", -1, 0};
    EXPECT_EQ(testNode.getChildStart(), -1);

    testNode.setChildStart(5);
    EXPECT_EQ(testNode.getChildStart(), 5);
}

TEST_F(QueryNodeTest, IncrementChildCount) {
    QueryNode testNode{4, QueryOperator::COMBINE, "", -1, 0};
    EXPECT_EQ(testNode.getChildCount(), 0);

    testNode.incrementChildCount();
    EXPECT_EQ(testNode.getChildCount(), 1);

    testNode.incrementChildCount();
    EXPECT_EQ(testNode.getChildCount(), 2);
}