#pragma once
#include "Token.h"
#include "Fragmenter.h"
#include "Scorer.h"
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <algorithm>

namespace Snippets {

/**
 * @brief Highlighter that fragments and scores document text.
 *
 * This Highlighter uses a Fragmenter to break the document into fragments and 
 * a Scorer to evaluate each token. Tokens that score positively are wrapped 
 * with highlight markers.
 */
class Highlighter {
public:
    /**
     * @param scorer A pointer to a Scorer implementation
     * @param fragmenter A pointer to a Fragmenter implementation
     * @param hi_start The start marker for highlighting (default "<B>").
     * @param hi_end The end marker for highlighting (default "</B>").
     * @param separator Separator string to append if a fragment is truncated (default "...").
     */
    Highlighter(Scorer* scorer, Fragmenter* fragmenter,
                const std::string& hi_start = "<B>",
                const std::string& hi_end = "</B>",
                const std::string& separator = "...")
        : scorer(scorer), fragmenter(fragmenter),
          hi_start(hi_start), hi_end(hi_end), separator(separator) {}

    /**
     * @brief Generates highlighted fragments from the given document text.
     * @param text The full document text.
     * @return A vector of highlighted fragments.
     */
    std::vector<std::string> getHighlightedFragments(const std::string& text);

    /**
     * @brief Returns the best fragment (highest-scoring) from the document.
     *
     * For simplicity, this example returns the first fragment.
     * In a more advanced implementation, you would compare fragment scores.
     *
     * @param text The full document text.
     * @return The best highlighted fragment.
     */
    std::string getBestFragment(const std::string& text);

private:
    Scorer* scorer;
    Fragmenter* fragmenter;
    std::string hi_start;
    std::string hi_end;
    std::string separator;

    // Utility: Simple whitespace-based tokenizer.
    std::vector<Token> tokenize(const std::string& text);

    // Utility: Build a fragment string from token pieces.
    std::string buildFragment(const std::string& text, size_t start, size_t end, 
                              const std::vector<std::string>& tokens);
        
};

} // namespace Snippets