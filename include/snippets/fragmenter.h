#pragma once
#include "Token.h"
#include <string>
#include <vector>
#include <cstddef>
#include <algorithm>

/**
 * @brief A basic fragmenter that breaks a documents text into fixed-size fragments.
 *
 * A Fragmenter is initialized with the full text and token list and then called
 * for each token to decide whether a new fragment should be started. 
 * The fragment boundary is determined by a fixed number of characters.
 */
class Fragmenter {
public:
    /**
     * @param fragmentSize The size (in characters) for each fragment.
     */
    explicit Fragmenter(size_t fragmentSize = 100)
        : fragmentSize(fragmentSize), currentFragmentEnd(0) {}

    void start(const std::string& /*text*/, const std::vector<Token>& /*tokens*/) {
        // Initialize the first fragment boundary.
        currentFragmentEnd = fragmentSize;
    }

    bool isNewFragment(const Token& token) {
        // If the token's end offset exceeds the current fragment boundary,
        // signal the start of a new fragment and update the boundary.
        if (token.end >= currentFragmentEnd) {
            currentFragmentEnd += fragmentSize;
            return true;
        }
        return false;
    }
    
private:
    size_t fragmentSize;
    size_t currentFragmentEnd;
};
