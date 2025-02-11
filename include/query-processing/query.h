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
 * @brief Processes a query string by tokenizing, correcting typos, and normalizing it.
 * 
 * @param rawQuery The raw query string to process.
 * @param dictionary The dictionary for typo detection.
 * @param tree The BK-Tree containing dictionary words.
 * @return A vector of corrected query tokens.
 */
TokenList processQuery(const std::string& rawQuery,
                       const Dictionary& dictionary, const bk::BKTree& tree);
} // namespace query
