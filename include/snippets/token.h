#pragma once
#include <string>

/**
 * @brief Represents a token with its text and character offsets in the original document.
 */
struct Token {
    std::string text;
    size_t start; // starting offset in the document
    size_t end;   // ending offset (non-inclusive)
};
