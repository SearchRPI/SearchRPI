#include "../../include/query-processing/queryNode.h"

namespace queryTree {

QueryNode::QueryNode(int nodeIndex, const std::string& operation,
    const std::string& value, int childStart, int childCount)
    : nodeIndex(nodeIndex), operation(operation), value(value),
    childStart(childStart), childCount(childCount) {
    type = value.empty() ? NodeType::OPERATION : NodeType::VALUE;
}

bool QueryNode::isOperation() const {
    return type == NodeType::OPERATION;
}

const std::string& QueryNode::getOperation() const {
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
