#include "snippets/highlighter.h"

namespace Snippets {

std::vector<std::string> Highlighter::getHighlightedFragments(const std::string& text) {
    // Tokenize the full document.
    auto tokens = tokenize(text);
    // Initialize the fragmenter.
    fragmenter->start(text, tokens);

    std::vector<std::string> fragments;
    std::vector<std::string> currentFragmentTokens;
    size_t fragmentStart = 0;

    // Start scoring for the first fragment.
    scorer->startFragment();

    // Process each token.
    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];

        // Check if we need to start a new fragment.
        if (i > 0 && fragmenter->isNewFragment(token)) {
            // Build and store the current fragment.
            std::string fragment = buildFragment(text, fragmentStart, tokens[i - 1].end, currentFragmentTokens);
            fragments.push_back(fragment);
            currentFragmentTokens.clear();
            fragmentStart = token.start;
            scorer->startFragment();
        }
        // Get token score.
        float tokenScore = scorer->getTokenScore(token);
        std::string tokenText = text.substr(token.start, token.end - token.start);
        // If the token scores positively, wrap it with highlight markers.
        if (tokenScore > 0.0f) {
            currentFragmentTokens.push_back(hi_start + tokenText + hi_end);
        } else {
            currentFragmentTokens.push_back(tokenText);
        }
    }
    // Finalize the last fragment.
    if (!currentFragmentTokens.empty()) {
        std::string fragment = buildFragment(text, fragmentStart, text.size(), currentFragmentTokens);
        fragments.push_back(fragment);
    }
    return fragments;
}

std::string Highlighter::getBestFragment(const std::string& text) {
    auto fragments = getHighlightedFragments(text);
    if (!fragments.empty())
        return fragments.front();
    return "";
}

std::vector<Token> Highlighter::tokenize(const std::string& text) {
    std::vector<Token> tokens;
    size_t pos = 0;
    while (pos < text.size()) {
        while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos])))
            pos++;
        if (pos >= text.size())
            break;
        size_t tokenStart = pos;
        while (pos < text.size() && !std::isspace(static_cast<unsigned char>(text[pos])))
            pos++;
        size_t tokenEnd = pos;
        tokens.push_back({ text.substr(tokenStart, tokenEnd - tokenStart), tokenStart, tokenEnd });
    }
    return tokens;
}

std::string Highlighter::buildFragment(const std::string& text, size_t start, size_t end, const std::vector<std::string>& tokens) {
    std::ostringstream oss;
    for (const auto& tok : tokens)
        oss << tok << " ";
    std::string frag = oss.str();
    if (!frag.empty() && frag.back() == ' ')
        frag.pop_back();
    // If the fragment doesn't reach the end of the text, append the separator.
    if (end < text.size())
        frag += separator;
    return frag;
}

}