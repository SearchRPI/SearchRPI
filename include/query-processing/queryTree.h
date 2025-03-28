#pragma once

/**
 ******************************************************************************
 * @file           : queryTree.h
 * @brief          : Efficient indexed query structure for ranking.
 ******************************************************************************
*/

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <string>
#include <functional>
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
    * @brief Retrieves a all the query tree nodes
    * @return Pointer to the requested QueryNodes
    */
    const std::vector<QueryNode>* getNodes() const { return &nodes; }

    /**
     * @brief Helper function for itterating through query tree
     */
    void forEachNodeWithDepth(
        const std::function<void(const QueryNode&, int depth)>& callback) const;


    /**
    * @brief Prints the structured query tree for debugging.
    */
    friend std::ostream& operator<<(std::ostream& os, const QueryTree& tree);

private:
    void addPhraseNodes(const queryTree::TokenList& tokens,
        const std::map<int, std::pair<std::string, std::vector<int>>>& phraseMap,
        std::unordered_set<int>& usedTokens, int& nodeIndex);

    void addTermNodes(const queryTree::TokenList& tokens,
                      const std::unordered_set<int>& usedTokens,
                      int& nodeIndex);

    std::vector<QueryNode> nodes;
};

} // namespace queryTree
