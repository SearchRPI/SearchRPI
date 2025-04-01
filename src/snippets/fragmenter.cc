#include "snippets/fragmenter.h"

namespace Snippets {

void Fragmenter::start(const std::string& text, const std::vector<Token>& tokens) {
    // Initialize the first fragment boundary.
    currentFragmentEnd = fragmentSize;
}

bool Fragmenter::isNewFragment(const Token& token) {
    // If the token's end offset exceeds the current fragment boundary,
    // signal the start of a new fragment and update the boundary.
    if (token.end >= currentFragmentEnd) {
        currentFragmentEnd += fragmentSize;
        return true;
    }
    return false;
}


}