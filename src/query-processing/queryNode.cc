#include "../../include/query-processing/queryNode.h"
#include <iostream>

namespace queryTree {

QueryNode::QueryNode(int nodeIndex, QueryOperator operation,
    const std::string& value, int childStart, int childCount)
    : nodeIndex(nodeIndex), operation(operation), value(value),
    childStart(childStart), childCount(childCount) {}



bool QueryNode::isOperation() const {
    return operation != QueryOperator::TEXT;
}

QueryOperator QueryNode::getOperation() const {
    return operation;
}

const std::string& QueryNode::getValue() const {
    return value;
}

int QueryNode::getChildStart() const {
    return childStart;
}

int QueryNode::getChildCount() const {
    return childCount;
}

int QueryNode::getNodeIndex() const {
    return nodeIndex;
}

} // namespace queryTree
