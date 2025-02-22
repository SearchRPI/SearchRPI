#pragma once

/**
  ******************************************************************************
  * @file           : query.h
  * @brief          : Processes input query and optimizes it for ranking.
  * @date           : 1/31/2025
  ******************************************************************************
*/

#include <string>
#include <vector>
#include <unordered_set>
#include "bkTree.h"
#include "queryTree.h"

/**
 * @namespace query
 * @brief Contains utility functions for processing queries.
 */
namespace query {

using Dictionary = std::unordered_set<std::string>;
using TokenList = std::vector<std::string>;

/**
 * @brief Loads a dictionary from a given file path.
 *
 * @param filepath The path to the file containing the dictionary.
 * @return A set of words loaded from the file.
 */
Dictionary loadDictionary(const std::string& filepath);

/**
 * @brief Processes a query string
 *
 * @param rawQuery The raw query string to process.
 * @param dictionary The dictionary for typo detection.
 * @param tree The BK-Tree containing dictionary words.
 * @param termDictionary The dictionary for identifying multi-word phrases.
 * @return A shared pointer to the root of the constructed query tree.
 */
std::shared_ptr<queryTree::QueryNode> processQuery(
    const std::string& rawQuery,
    const Dictionary& dictionary,
    const bk::BKTree& tree,
    const queryTree::TermDictionary& termDictionary
);
} // namespace query
