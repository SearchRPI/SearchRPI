#pragma once
#include "snippets/Scorer.h"
#include "snippets/Token.h"
#include <set>
#include <string>
#include <cctype>

/**
 * @brief A simple scorer for tokens and fragments that gives a positive score 
 * for tokens matching query terms.
 *
 * In this simplified version, if a token (normalized to lowercase) is in the 
 * query term set, its score is set proportional to its length.
 */
class Scorer {
public:
    /**
     * @param queryTerms A set of query terms (assumed to be pre-normalized to lowercase).
     */
    Scorer(const std::set<std::string>& queryTerms)
        : queryTerms(queryTerms), totalScore(0.0f) {}

    void startFragment() {
        totalScore = 0.0f;
    }

    float getTokenScore(const Token& token) {
        std::string lowerToken = toLower(token.text);
        if (queryTerms.find(lowerToken) != queryTerms.end()) {
            float score = static_cast<float>(token.text.size());
            totalScore += score;
            return score;
        }
        return 0.0f;
    }

    float getFragmentScore() const {
        return totalScore;
    }

private:
    std::set<std::string> queryTerms;
    float totalScore;

    // Utility: converts a string to lowercase.
    std::string toLower(const std::string& str) const {
        std::string lower;
        lower.reserve(str.size());
        for (char c : str) {
            lower.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
        return lower;
    }
};
