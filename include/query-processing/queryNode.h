#pragma once

/**
  ******************************************************************************
  * @file           : queryNode.h
  * @brief          : Node structure for indexed query representation.
  ******************************************************************************
*/

#include <string>
#include "queryOperator.h"

/**
 * @namespace queryTree
 * @brief Contains the core structures for query processing.
 */
namespace queryTree {

/**
 * @class QueryNode
 * @brief Represents a node in the indexed query representation.
 */
class QueryNode {
public:
    /**
     * @brief Constructs a query node.
     * @param nodeIndex The position of this node in the array.
     * @param operation The query operation as an enum.
     * @param value The term value (for value nodes).
     * @param childStart The index of the first child in the array (-1 if none).
     * @param childCount The number of children this node has.
     */
    QueryNode(int nodeIndex, QueryOperator operation, const std::string& value,
              int childStart, int childCount);

    /**
     * @brief Checks if this node is an operation node.
     * @return True if this is an operation node, false if it is a value node.
     */
    bool isOperation() const;

    /**
     * @brief Gets the operation associated with this node.
     * @return The operator enum
     */
    QueryOperator getOperation() const;

    /**
     * @brief Gets the value associated with this node.
     * @return The value string, NULL otherwise
     */
    const std::string& getValue() const;

    /**
     * @brief Gets the index of the first child.
     * @return The index of the first child node in the array (-1 if none).
     */
    int getChildStart() const;

    /**
     * @brief Gets the number of children this node has.
     * @return The number of child nodes.
     */
    int getChildCount() const;

    /**
     * @brief Gets the index of this node in the array.
     * @return The index of this node.
     */
    int getNodeIndex() const;

private:
    int nodeIndex;
    QueryOperator operation;
    std::string value;
    int childStart;
    int childCount;
};

} // namespace queryTree