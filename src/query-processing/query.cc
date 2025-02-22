#include "../../include/query-processing/query.h"
#include "../../include/query-processing/stemmer.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <climits>

// Use definitions from the query namespace
using query::Dictionary;
using query::TokenList;

/**
 * @brief Checks if a given word is a stop word.
 *
 * @param word The word to check.
 * @return True if the word is a stop word, false otherwise.
 */
// TODO: UPDATE TO READ FROM STOPWORDS.TXT
bool isStopWord(const std::string& word) {
    static const Dictionary stopWords = {
        "the", "of", "to", "a", "and", "in", "said", "for", "that", "was", "on",
        "he", "is", "with", "at", "by", "it", "from", "as", "be", "were", "an",
        "have", "his", "but", "has", "are", "not", "who", "they", "its", "had",
        "will", "would", "about", "i", "been", "this", "their", "new", "or",
        "which", "we", "more", "after", "us", "percent", "up", "one", "people",
        "what"
    };
    return stopWords.find(word) != stopWords.end();
}

/**
 * @brief Converts a word to lowercase.
 *
 * @param word The word to convert.
 * @return The lowercase version of the word.
 */
std::string toLower(const std::string& word) {
    std::string result = word;
    std::transform(
       result.begin(), result.end(), result.begin(), [](unsigned char c) {
       return std::tolower(c);
    });
    return result;
}

/**
 * @brief Removes punctuation from a given word.
 *
 * @param word The word to process.
 * @return The word without punctuation.
 */
std::string removePunctuation(std::string word) {
    word.erase(std::remove_if(word.begin(), word.end(),
              [](unsigned char c) { return !std::isalnum(c); }), word.end());
    return word;
}

/**
 * @brief Tokenizes a raw query string into individual words.
 *
 * @param rawQuery The raw query string to tokenize.
 * @return A vector of tokens (words) from the query.
 */
TokenList tokenize(const std::string& rawQuery) {
    TokenList tokens;
    std::istringstream stream(rawQuery);
    std::string token;

    while (stream >> token) {
        token = toLower(token);
        token = removePunctuation(token);
        if (!isStopWord(token)) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

/**
 * @brief Finds the closest word in the dictionary to a given word.
 *
 * @param word The word to find a match for.
 * @param tree The BK-Tree containing dictionary words.
 * @param dictionary The set of dictionary words.
 * @return The closest matching word from the dictionary.
 */
std::string findClosestWord(const std::string& word, const bk::BKTree& tree) {
    return tree.findClosest(word);
}

/**
 * @brief Finds typos in a list of tokens and suggests corrections.
 *
 * @param tokens The list of tokens to check.
 * @param tree The BK-Tree containing dictionary words.
 * @param dictionary The dictionary for typo detection.
 * @return A vector of corrected words.
 */
TokenList findSuggestion(const TokenList& tokens,
                         const bk::BKTree& tree,
                         const Dictionary& dictionary) {
    TokenList suggestions;

    for (const std::string& token : tokens) {
        if (dictionary.find(token) == dictionary.end()) {
            suggestions.emplace_back(tree.findClosest(token));
        } else {
            suggestions.emplace_back(token);  // Keep correct words
        }
    }

    return suggestions;
}

namespace query {

std::shared_ptr<queryTree::QueryNode> processQuery(
    const std::string& rawQuery,
    const Dictionary& dictionary,
    const bk::BKTree& tree,
    const queryTree::TermDictionary& termDictionary
) {
    // Tokenization
    TokenList tokens = tokenize(rawQuery);

    // Spelling correction
    TokenList correctedTokens = findSuggestion(tokens, tree, dictionary);

    // Normalization: punctuation removal & stemming
    TokenList processedTokens;
    for (std::string& token : correctedTokens) {
        token = removePunctuation(token);
        processedTokens.push_back(stemmer::stem(token));
    }

    // Build and return the query tree
    return queryTree::buildQueryTree(processedTokens, termDictionary);
}

// NOTE: This function might fit better elsewhere
Dictionary loadDictionary(const std::string& filepath) {
    Dictionary dictionary;
    std::ifstream file(filepath);
    std::string word;

    if (!file) {
        std::cerr << "Error: Unable to open file " << filepath << std::endl;
        return dictionary;
    }

    while (std::getline(file, word)) {
        word.erase(0, word.find_first_not_of(" \t\r\n"));
        word.erase(word.find_last_not_of(" \t\r\n") + 1);
        if (!word.empty()) {
            dictionary.insert(toLower(word));
        }
    }

    file.close();
    return dictionary;
}

} // namespace query
