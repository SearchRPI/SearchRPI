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
 * @brief Contains utility functions for processing queries, including tokenization, typo detection, and query expansion.
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
 * @brief Checks if a given word is a stop word.
 * 
 * @param word The word to check.
 * @return True if the word is a stop word, false otherwise.
 */
bool isStopWord(const std::string& word);

/**
 * @brief Converts a word to lowercase.
 * 
 * @param word The word to convert.
 * @return The lowercase version of the word.
 */
std::string toLower(const std::string& word);

/**
 * @brief Removes punctuation from a given word.
 * 
 * @param word The word to process.
 * @return The word without punctuation.
 */
std::string removePunctuation(std::string word);

/**
 * @brief Tokenizes a raw query string into individual words.
 * 
 * @param rawQuery The raw query string to tokenize.
 * @return A vector of tokens (words) from the query.
 */
TokenList tokenize(const std::string& rawQuery);

/**
 * @brief Finds the closest word in the dictionary to a given word.
 * 
 * @param word The word to find a match for.
 * @param tree The BK-Tree containing dictionary words.
 * @param dictionary The set of dictionary words.
 * @return The closest matching word from the dictionary.
 */
std::string findClosestWord(const std::string& word, const bk::BKTree& tree);

/**
 * @brief Finds typos in a list of tokens and suggests corrections.
 * 
 * @param tokens The list of tokens to check.
 * @param tree The BK-Tree containing dictionary words.
 * @param dictionary The dictionary for typo detection.
 * @return A vector of corrected words.
 */
TokenList findSuggestion(const TokenList& tokens, const bk::BKTree& tree,
                         const Dictionary& dictionary);

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
