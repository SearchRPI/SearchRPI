#include "../../include/query-processing/queryTree.h"
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Loads query operators from a file into an unordered_set.
 * @param filename The path to the operator list file.
 * @return A set containing all recognized query operators.
 */
std::unordered_set<std::string> loadOperators(const std::string& filename) {
    std::unordered_set<std::string> operators;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open operator file: " << filename << std::endl;
        return operators;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            operators.insert(line);
        }
    }

    return operators;
}

/**
 * @brief Extracts an operation and its parameters from a token.
 * @param token The input token (e.g., "#dirichlet:1500").
 * @param operation The extracted operation name.
 * @param value The extracted value (if any).
 */
void extractOperationDetails(const std::string& token, std::string& operation, std::string& value) {
    size_t paramPos = token.find(':');
    if (paramPos != std::string::npos) {
        operation = token.substr(0, paramPos);
        value = token.substr(paramPos + 1);
    } else {
        operation = token;
        value = "";
    }
}

/**
 * @brief Finds multi-word phrases in the token list based on a dictionary.
 * @param tokens The list of query tokens.
 * @param dict The dictionary containing valid phrases.
 * @return A set of recognized phrases.
 */
std::unordered_set<std::string> findPhrases(const TokenList& tokens,
                                            const TermDictionary& dict) {
    std::unordered_set<std::string> foundPhrases;

    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string phrase = tokens[i];

        for (size_t j = i + 1; j < tokens.size(); ++j) {
            phrase += " " + tokens[j];

            if (dict.find(phrase) != dict.end()) {
                foundPhrases.insert(phrase);
                i = j;  // Skip the next token since it's part of a phrase
                break;
            }
        }
    }

    return foundPhrases;
}

namespace queryTree {
bool QueryTree::isOperation(const std::string& token) {
    std::unordered_set<std::string> OPERATORS = loadOperators("operators.txt");

    bool isOperation(const std::string& token) {
        return OPERATORS.find(token) != OPERATORS.end()
            || token.rfind("#od:", 0) == 0
            || token.rfind("#uw:", 0) == 0;
    }
}

QueryTree::QueryTree(const TokenList& tokens, const TermDictionary& dict) {
    if (tokens.empty()) return;

    nodes.reserve(tokens.size()); // Preallocate for efficiency

    std::unordered_set<std::string> usedTokens = findPhrases(tokens, dict);
    int nodeIndex = 0;

    // Process tokens and create nodes
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (usedTokens.find(tokens[i]) != usedTokens.end()) {
            continue;
        }

        std::string operation, value;
        extractOperationDetails(tokens[i], operation, value);

        nodes.emplace_back(nodeIndex++, operation, value, -1, 0);
    }
}

const QueryNode* QueryTree::getNode(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= nodes.size()) {
        return nullptr;
    }
    return &nodes[index];
}

void QueryTree::print() const {
    for (const auto& node : nodes) {
        std::cout << "Node " << node.getNodeIndex() << ": " << node.getOperation();
        if (!node.getValue().empty()) {
            std::cout << " (" << node.getValue() << ")";
        }
        std::cout << std::endl;
    }
}

} // namespace queryTree
