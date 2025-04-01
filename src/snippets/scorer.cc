#include "snippets/scorer.h"

namespace Snippets {

float Scorer::getTokenScore(const Token& token) {
    std::string lowerToken = toLower(token.text);
    if (queryTerms.find(lowerToken) != queryTerms.end()) {
        float score = static_cast<float>(token.text.size());
        totalScore += score;
        return score;
    }
    return 0.0f;
}

}