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
        "#combine", "#weight", "#and", "#or", "#max", "#dirichlet", "#bm25",
        "#lengths"
    };

    return operators.find(token) != operators.end() 
        || token.rfind("#od:", 0) == 0
        || token.rfind("#uw:", 0) == 0;
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
    
    // Handle proximity operators like #od:N or #uw:N
    else if (token.rfind("#od:", 0) == 0 || token.rfind("#uw:", 0) == 0) {
        return std::make_shared<queryTree::QueryNode>(token);
    }

    // Handle weighted queries #weight(2.0 fish)
    else if (token.rfind("#weight", 0) == 0) {
        return std::make_shared<queryTree::QueryNode>("#weight");
    }

    // Handle parameterized operators like #dirichlet:1500(term)
    else if (token.find(':') != std::string::npos) {
        size_t pos = token.find(':');
        std::string op = token.substr(0, pos);
        std::string val = token.substr(pos + 1);
        return std::make_shared<queryTree::QueryNode>(op, val);
    }

    // Default case: treat it as a term
    return std::make_shared<queryTree::QueryNode>("#text", token);
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
    std::unordered_set<std::string> foundPhrases;

    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string phrase = tokens[i];

        for (size_t j = i + 1; j < tokens.size(); ++j) {
            phrase += " " + tokens[j];

            if (dict.find(phrase) != dict.end() && foundPhrases.find(phrase) == foundPhrases.end()) {
                phrases.push_back(phrase);
                foundPhrases.insert(phrase);
                i = j;  // Skip the next token as it's part of the phrase
                break;
            }
        }
    }

    return phrases;
}

namespace queryTree {

std::shared_ptr<queryTree::QueryNode> buildQueryTree(const TokenList& tokens,
                                                     const TermDictionary& dict) {
    if (tokens.empty()) return nullptr;

    auto root = std::make_shared<QueryNode>("#combine");

    std::vector<std::string> phraseTokens = findPhrases(tokens, dict);
    std::unordered_set<std::string> usedTokens(phraseTokens.begin(), phraseTokens.end());

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (usedTokens.find(tokens[i]) != usedTokens.end()) {
            continue;  // Skip tokens that are part of a detected phrase
        }

        auto node = createNode(tokens[i]);

        // If the token is the start of a phrase, build an #od:1() node
        if (i < tokens.size() - 1) {
            std::string phrase = tokens[i] + " " + tokens[i + 1];

            if (dict.find(phrase) != dict.end()) {
                auto phraseNode = std::make_shared<QueryNode>("#od:1");
                phraseNode->children.push_back(createNode(tokens[i]));
                phraseNode->children.push_back(createNode(tokens[i + 1]));

                root->children.push_back(phraseNode);
                ++i;  // Skip the next token as it's part of the phrase
                continue;
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
