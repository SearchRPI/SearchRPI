#include "../../include/query-processing/query.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <climits>

namespace query {

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
            dictionary.insert(word);
        }
    }

    file.close();
    return dictionary;
}

bool isStopWord(const std::string& word) {
    static const Dictionary stopWords = {
        "the", "of", "to", "a", "and", "in", "said", "for", "that", "was", "on",
        "he", "is", "with", "at", "by", "it", "from", "as", "be", "were", "an",
        "have", "his", "but", "has", "are", "not", "who", "they", "its", "had",
        "will", "would", "about", "i", "been", "this", "their", "new", "or",
        "which", "we", "more", "after", "us", "percent", "up", "one", "people"
    };
    return stopWords.find(word) != stopWords.end();
}

std::string toLower(const std::string& word) {
    std::string result = word;
    std::transform(
       result.begin(), result.end(), result.begin(), [](unsigned char c) {
       return std::tolower(c);
    });
    return result;
}

std::string removePunctuation(std::string word) {
    word.erase(std::remove_if(word.begin(), word.end(),
              [](unsigned char c) { return !std::isalnum(c); }), word.end());
    return word;
}

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

std::string findClosestWord(const std::string& word, const bk::BKTree& tree) {
    return tree.findClosest(word);
}

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

TokenList processQuery(const std::string& rawQuery,
                       const Dictionary& dictionary,
                       const bk::BKTree& tree) {
    TokenList tokens = tokenize(rawQuery);
    TokenList correctedTokens = findSuggestion(tokens, tree, dictionary);
    // TODO: Apply stemming here if needed
    return correctedTokens;
}

} // namespace query
