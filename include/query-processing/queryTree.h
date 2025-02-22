#pragma once

/**
  ******************************************************************************
  * @file           : queryTree.h
  * @brief          : Processes query into a tree for optimized ranking.
  * @date           : 2/21/2025
  ******************************************************************************
*/

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/**
 * @namespace query
 * @brief Contains the Node and structure for a query tree and debugging help.
 */
namespace queryTree {

using TermDictionary = std::unordered_map<std::string, std::vector<std::string>>;
using TokenList = std::vector<std::string>;

/**
 * @brief Represents a node in the structured query tree.
 */
struct QueryNode {
    std::string operation;
    std::vector<std::shared_ptr<QueryNode>> children;
    std::string value;

    /**
     * @brief Constructs a QueryNode with an operation only.
     * @param op The query operator (e.g., "#combine").
     */
    explicit QueryNode(const std::string& op);

    /**
     * @brief Constructs a QueryNode with an operation and value.
     * @param op The query operator
     * @param val The term associated with this node.
     */
    QueryNode(const std::string& op, const std::string& val);
};

/**
 * @brief Builds a structured query tree from a processed list of tokens.
 * @param tokens The list of processed tokens.
 * @param dict The dictionary object to check for phrases.
 * @return A shared pointer to the root of the query tree.
 * @note https://sourceforge.net/p/lemur/wiki/Galago%20Query%20Language/
 */
std::shared_ptr<QueryNode> buildQueryTree(const TokenList& tokens, 
                                          const TermDictionary& dict);

/**
 * @brief Prints the structured query tree for debugging.
 * @param root The root of the query tree.
 * @param depth The indentation level (default is 0).
 */
void printQueryTree(const std::shared_ptr<QueryNode>& root, int depth = 0);
} // namespace queryTree
