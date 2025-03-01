#pragma once

/**
  ******************************************************************************
  * @file           : queryTree.h
  * @brief          : Efficient indexed query structure for ranking.
  ******************************************************************************
*/

#include <vector>
#include <unordered_map>
#include <string>
#include "queryNode.h"

/**
 * @namespace queryTree
 * @brief Contains the core structures for query processing.
 */
namespace queryTree {

using TermDictionary = std::unordered_map<std::string, std::vector<std::string>>;
using TokenList = std::vector<std::string>;

/**
 * @class QueryTree
 * @brief Represents a structured, indexed query for traversal and ranking.
 */
class QueryTree {
public:
  /**
    * @brief Constructs a QueryTree from tokenized input.
    * @param tokens The list of processed tokens.
    * @param dict The dictionary for phrase detection.
    */
  QueryTree(const TokenList& tokens, const TermDictionary& dict);

  /**
    * @brief Retrieves a specific node by index.
    * @param index The index of the node.
    * @return Pointer to the requested QueryNode, or nullptr if out of bounds.
    */
  const QueryNode* getNode(int index) const;

  /**
    * @brief Prints the structured query tree for debugging.
    */
  void print() const;

private:
  /**
  * @brief Checks if a token is a recognized query operation.
  * @param token The token to check.
  * @return True if the token is an operator, false otherwise.
  */
  bool QueryTree::isOperation(const std::string& token);

    std::vector<QueryNode> nodes;
};

} // namespace queryTree
