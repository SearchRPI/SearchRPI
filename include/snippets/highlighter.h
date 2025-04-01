#pragma once
#include "snippets/token.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <set>
#include <vector>

namespace Snippets {

// A simple whitespace tokenizer that also records character offsets.
inline std::vector<Token> tokenize(const std::string& text) {
    std::vector<Token> tokens;
    size_t pos = 0;
    while (pos < text.size()) {
        // Skip any whitespace
        while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos])))
            pos++;
        if (pos >= text.size())
            break;
        size_t tokenStart = pos;
        // Advance until next whitespace
        while (pos < text.size() && !std::isspace(static_cast<unsigned char>(text[pos])))
            pos++;
        size_t tokenEnd = pos;
        tokens.push_back({ text.substr(tokenStart, tokenEnd - tokenStart), tokenStart, tokenEnd });
    }
    return tokens;
}

// Convert a string to lowercase.
inline std::string toLower(const std::string& str) {
    std::string lower;
    lower.reserve(str.size());
    for (char c : str) {
        lower.push_back(std::tolower(static_cast<unsigned char>(c)));
    }
    return lower;
}

/**
 * @brief Highlighter for generating document snippets with highlighted query terms.
 *
 * This implementation re-analyzes the document text at query time (i.e. without precomputed term vectors).
 * It uses a simple whitespace tokenizer and centers a snippet around the first query match.
 */
class Highlighter {
public:
    /**
     * @param text The full document text.
     * @param queryTerms A set of query terms (ideally pre-normalized, e.g. lowercased) to highlight.
     * @param snippetLength Maximum length (in characters) for the returned snippet.
     * @param hi_start The highlight start marker (e.g. "<B>").
     * @param hi_end The highlight end marker (e.g. "</B>").
     * @param omit (Optional) A string appended to the snippet if it is truncated.
     *
     * @returns A highlighted snippet with query terms wrapped in the given markers.
     */
    std::string getSnippet(const std::string& text,
                           const std::set<std::string>& queryTerms,
                           size_t snippetLength,
                           const std::string& hi_start,
                           const std::string& hi_end,
                           const std::string& omit = "...")
    {
        // First, tokenize the full document text.
        auto tokens = tokenize(text);

        // Prepare a normalized (lowercase) version of the query terms.
        std::set<std::string> normQuery;
        for (const auto& term : queryTerms)
            normQuery.insert(toLower(term));

        bool matchFound = false;
        size_t snippetStart = 0, snippetEnd = text.size();

        // Find the first token that matches any query term.
        for (const auto& token : tokens) {
            if (normQuery.find(toLower(token.text)) != normQuery.end()) {
                matchFound = true;
                // Center the snippet window around the matching token.
                // We try to show snippetLength characters in total.
                if (token.start > snippetLength / 2)
                    snippetStart = token.start - snippetLength / 2;
                else
                    snippetStart = 0;
                snippetEnd = std::min(text.size(), snippetStart + snippetLength);
                break;
            }
        }

        // If no matching token is found, use the beginning of the document.
        if (!matchFound) {
            snippetStart = 0;
            snippetEnd = std::min(text.size(), snippetLength);
        }

        // Extract the snippet text.
        std::string snippet = text.substr(snippetStart, snippetEnd - snippetStart);

        // Re-tokenize the snippet so we can insert highlight markers.
        auto snippetTokens = tokenize(snippet);

        // Build the highlighted snippet.
        std::string highlighted;
        size_t pos = 0;
        for (const auto& token : snippetTokens) {

            if (token.start > pos)
                highlighted += snippet.substr(pos, token.start - pos);

            std::string normToken = toLower(token.text);
            if (normQuery.find(normToken) != normQuery.end()) {
                highlighted += hi_start + token.text + hi_end;
            } else {
                highlighted += token.text;
            }
            pos = token.end;
        }

        if (pos < snippet.size())
            highlighted += snippet.substr(pos);

        // If the snippet doesn't contain the entire document, append the omission marker.
        if (snippetEnd < text.size())
            highlighted += omit;

        return highlighted;
    }
};

} // namespace Snippets