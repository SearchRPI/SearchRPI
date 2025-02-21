#include "../../include/query-processing/queryTree.h"
#include <iostream>
#include <stack>
#include <unordered_set>

queryTree::QueryNode::QueryNode(const std::string& op) : operation(op) {}

queryTree::QueryNode::QueryNode(const std::string& op, const std::string& val)
    : operation(op), value(val) {}

/**
 * @brief Checks if a given token is a recognized query operator.
 *
 * @param token The token to check.
 * @return True if the token is an operator, false otherwise.
 */
bool isOperator(const std::string& token) {
    static const std::unordered_set<std::string> operators = {
        "#combine", "#syn", "#od:1", "#uw:3", "#and", "#or"
    };

    return operators.find(token) != operators.end();
}

/**
 * @brief Creates a QueryNode based on the token type.
 *
 * @param token The token to process.
 * @return A shared pointer to the created QueryNode.
 */
std::shared_ptr<queryTree::QueryNode> createNode(const std::string& token) {
    if (isOperator(token)) {
        return std::make_shared<queryTree::QueryNode>(token);
    }
    return std::make_shared<queryTree::QueryNode>("term", token);
}

/**
 * @brief Retrieves synonyms for a given term from the thesaurus.
 *
 * @param term The input term for which synonyms are needed.
 * @param thesaurus The dictionary containing term-to-synonym mappings.
 * @return A list of synonyms for the given term. Empty vector if none found.
 */
std::vector<std::string> findSynonyms(const std::string& term,
                                  const queryTree::TermDictionary& thesaurus) {
    auto it = thesaurus.find(term);
    if (it != thesaurus.end()) {
        return it->second;
    }
    return {};
}

/**
 * @brief Checks if a sequence of tokens forms a phrase in the dictionary.
 *
 * @param tokens The list of processed query tokens.
 * @param dict The dictionary containing predefined phrases.
 * @return A list of recognized phrases found in the token list.
 */
std::vector<std::string> findPhrases(const queryTree::TokenList& tokens,
                                     const queryTree::TermDictionary& dict) {
    std::vector<std::string> phrases;

    // FIXME: O(n^2) Make faster if required.
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string phrase = tokens[i];

        for (size_t j = i + 1; j < tokens.size(); ++j) {
            // Build phrase incrementally.
            phrase += " " + tokens[j];

            if (dict.find(phrase) != dict.end()) {
                phrases.push_back(phrase);
            }
        }
    }

    return phrases;
}

namespace queryTree {

std::shared_ptr<QueryNode> buildQueryTree(const TokenList& tokens,
                                          const TermDictionary& dict,
                                          const TermDictionary& thesaurus) {
    if (tokens.empty()) return nullptr;

    auto root = std::make_shared<QueryNode>("#combine");

    for (const auto& token : tokens) {
        auto node = createNode(token);

        if (!isOperator(token)) {
            auto synonyms = findSynonyms(token, thesaurus);
            if (!synonyms.empty()) {
                auto synonymNode = std::make_shared<QueryNode>("#syn");
                synonymNode->children.push_back(node);
                for (const auto& synonym : synonyms) {
                    synonymNode->children.push_back(std::make_shared<QueryNode>("term", synonym));
                }
                node = synonymNode;
            }
        }

        root->children.push_back(node);
    }

    return root;
}

void printQueryTree(const std::shared_ptr<QueryNode>& root, int depth) {
    if (!root) return;

    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << root->operation;
    if (!root->value.empty()) {
        std::cout << " (" << root->value << ")";
    }
    std::cout << std::endl;

    for (const auto& child : root->children) {
        printQueryTree(child, depth + 1);
    }
}

} // namespace queryTree
