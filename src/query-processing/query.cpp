#include "query.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <climits>

namespace query {

// Load dictionary from a file
std::unordered_set<std::string> loadDictionary(const std::string& filepath) {
    std::unordered_set<std::string> dictionary;
    std::ifstream file(filepath);
    std::string word;

    if (!file) {
        std::cerr << "Error: Unable to open file " << filepath << std::endl;
        return dictionary;
    }

    while (std::getline(file, word)) {
        dictionary.insert(word);
    }

    file.close();
    return dictionary;
}

// Check if a word is a stop word
bool isStopWord(const std::string& word) {
    static const std::unordered_set<std::string> stopWords = {
        "the", "of", "to", "a", "and", "in", "said", "for", "that", "was", "on",
        "he", "is", "with", "at", "by", "it", "from", "as", "be", "were", "an",
        "have", "his", "but", "has", "are", "not", "who", "they", "its", "had",
        "will", "would", "about", "i", "been", "this", "their", "new", "or",
        "which", "we", "more", "after", "us", "percent", "up", "one", "people"
    };
    return stopWords.find(word) != stopWords.end();
}

// Convert a word to lowercase
std::string toLower(const std::string& word) {
    std::string result = word;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}

// Remove punctuation
std::string removePunctuation(const std::string& word) {
    std::string result;
    std::copy_if(word.begin(), word.end(), std::back_inserter(result), [](unsigned char c) {
        return std::isalnum(c) || std::isspace(c);
    });
    return result;
}

// Tokenize a query string
std::vector<std::string> tokenize(const std::string& rawQuery) {
    std::vector<std::string> tokens;
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

// Calculate edit distance
int editDistance(const std::string& a, const std::string& b) {
    size_t n = a.size(), m = b.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    for (size_t i = 0; i <= n; ++i) dp[i][0] = i;
    for (size_t j = 0; j <= m; ++j) dp[0][j] = j;

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    return dp[n][m];
}

// Find the closest word in the dictionary
std::string findClosestWord(const std::string& word, const std::unordered_set<std::string>& dictionary) {
    std::string closest_word = word;
    int min_distance = INT_MAX;

    for (const auto& dict_word : dictionary) {
        int distance = editDistance(word, dict_word);
        if (distance < min_distance) {
            min_distance = distance;
            closest_word = dict_word;
        }
    }
    return closest_word;
}

// Find typos and suggestions
std::vector<std::string> findSuggestion(const std::vector<std::string>& tokens,
                                        const std::unordered_set<std::string>& dictionary
) {
    std::vector<std::string> suggestion;

    for (const std::string& token : to>kens) {
        if (dictionary.find(token) == dictionary.end()) {  
            // FIXME: Make this not O(dict^2)
            std::string suggestion = findClosestWord(token, dictionary);
            suggestion.emplace_back(suggestion);
        }
    }

    return suggestion;
}

// Process query
std::vector<std::string> processQuery(const std::string& rawQuery,
                                      const std::unordered_set<std::string>& dictionary
) {
    std::vector<std::string> tokens = tokenize(rawQuery, dictionary);
    std::vector<std::string> suggestion = findSuggestion(tokens, dictionary);
    // stemming
    return ;
}

} // namespace query
