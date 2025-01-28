#pragma once

/**
  ******************************************************************************
  * @file           : query.hpp
  * @brief          : Processes input query and optimizes it for ranking.
  * @date           : 1/28/2025
  ******************************************************************************
*/

#include <string>
#include <vector>
#include <unordered_set>
// #include <libstemmer.h>

/**
 * @namespace query
 * @brief Contains utility functions for processing queries, including tokenization, typo detection, and query expansion.
 */
namespace query {

    /**
     * @brief Loads a dictionary from a given file path.
     * 
     * @param filepath The path to the file containing the dictionary.
     * @return A set of words loaded from the file.
     */
    std::unordered_set<std::string> loadDictionary(const std::string& filepath);

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
    std::string removePunctuation(const std::string& word);

    /**
     * @brief Tokenizes a raw query string into individual words.
     * 
     * @param rawQuery The raw query string to tokenize.
     * @param stopWords A set of stop words to exclude during tokenization.
     * @return A vector of tokens (words) from the query.
     */
    std::vector<std::string> tokenize(const std::string& rawQuery, const std::unordered_set<std::string>& stopWords);

    /**
     * @brief Calculates the Levenshtein distance (edit distance) between two strings.
     * 
     * @param a The first string.
     * @param b The second string.
     * @return The edit distance between the two strings.
     */
    int editDistance(const std::string& a, const std::string& b);

    /**
     * @brief Finds the closest word in the dictionary to a given word based on edit distance.
     * 
     * @param word The word to find a match for.
     * @param dictionary The set of dictionary words to search.
     * @return The closest matching word from the dictionary.
     */
    std::string findClosestWord(const std::string& word, const std::unordered_set<std::string>& dictionary);

    /**
     * @brief Finds typos in a list of tokens and provides suggestions for correction.
     * 
     * @param tokens The list of tokens to check.
     * @param dictionary The dictionary to use for typo detection.
     * @return A vector of pairs, where each pair contains a typo and its suggested correction.
     */
    std::vector<std::string> findTypos(const std::vector<std::string>& tokens, const std::unordered_set<std::string>& dictionary);

    /**
     * @brief Processes a query string, tokenizes it, and identifies typos with suggestions.
     * 
     * @param rawQuery The raw query string to process.
     * @param dictionary The dictionary to use for typo detection.
     * @return A vector of pairs, where each pair contains a typo and its suggested correction.
     */
    std::vector<std::pair<std::string, std::string>> processQuery(const std::string& rawQuery, const std::unordered_set<std::string>& dictionary);

}
