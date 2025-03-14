#include "../../include/query-processing/queryTree.h"
#include "../../include/query-processing/queryOperator.h"
#include <unordered_set>
#include <map>
#include <fstream>
#include <iostream>
#include <string>

/**
* @brief Converts a string token into its corresponding QueryOperator enum.
* @param token The operator string.
* @return The corresponding QueryOperator value.
*/
queryTree::QueryOperator getOperatorType(const std::string& token) {
    auto it = queryTree::OPERATOR_MAP.find(token);

    if (it ==  queryTree::OPERATOR_MAP.end()) {
        return queryTree::QueryOperator::TEXT;
    }

    return it->second;
}

/**
 * @brief Extracts the parameter from an operator token.
 * @param token The input token (e.g., "#dirichlet:1500").
 * @return The extracted value (if any), otherwise an empty string.
 */
std::string extractParameter(const std::string& token) {
    size_t paramPos = token.find(':');
    return (paramPos != std::string::npos) ? token.substr(paramPos + 1) : "";
}

/**
 * @brief Finds bi-word phrases in the token list based on a dictionary.
 * @param tokens The list of query tokens.
 * @param dict The dictionary containing valid phrases.
 * @return map<int, pair<string,vector<int>>> A set of recognized phrases.
 */
std::map<int, std::pair<std::string, std::vector<int>>> findPhrases(
    const queryTree::TokenList& tokens,
    const queryTree::TermDictionary& dict) {
    std::map<int, std::pair<std::string, std::vector<int>>> phraseMap;

    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        std::string phrase = tokens[i] + " " + tokens[i + 1];

        if (dict.find(phrase) != dict.end()) {
            phraseMap[i] = {phrase, {static_cast<int>(i), static_cast<int>(i + 1)}};
            ++i;  // Skip the next token as it's part of a phrase
        }
    }

    return phraseMap;
}


namespace queryTree {

void QueryTree::addPhraseNodes(const queryTree::TokenList& tokens,
    const std::map<int, std::pair<std::string, std::vector<int>>>& phraseMap,
    std::unordered_set<int>& usedTokens, int& nodeIndex) {
    for (const auto& [startIndex, phraseData] : phraseMap) {
        const std::string& phrase = phraseData.first;
        const std::vector<int>& tokenIndexes = phraseData.second;

        // Attach the OD node to the root (COMBINE node, index 0)
        int phraseNodeIndex = nodeIndex++;
        nodes.emplace_back(phraseNodeIndex, QueryOperator::OD, phrase, 0, tokenIndexes.size());

        // Add child term nodes under OD
        for (int idx : tokenIndexes) {
            usedTokens.insert(idx);
            nodes.emplace_back(nodeIndex++, QueryOperator::TEXT, tokens[idx], phraseNodeIndex, 0);
        }
    }
}


void QueryTree::addTermNodes(const queryTree::TokenList& tokens,
                             const std::unordered_set<int>& usedTokens,
                             int& nodeIndex) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (usedTokens.find(i) != usedTokens.end() || tokens[i].empty()) continue;

        QueryOperator opType = getOperatorType(tokens[i]);
        nodes.emplace_back(nodeIndex++, opType, tokens[i], -1, 0);
    }
}

QueryTree::QueryTree(const queryTree::TokenList& tokens,
                     const queryTree::TermDictionary& dict) {
    if (tokens.empty()) return;

    nodes.reserve(tokens.size()); // Preallocate for efficiency

    int nodeIndex = 0;
    nodes.emplace_back(nodeIndex++, QueryOperator::COMBINE, "", -1, tokens.size());

    // Find phrases and their positions
    auto phraseMap = findPhrases(tokens, dict);
    std::unordered_set<int> usedTokens;

    // add phrase nodes
    addPhraseNodes(tokens, phraseMap, usedTokens, nodeIndex);

    // Add individual term nodes
    addTermNodes(tokens, usedTokens, nodeIndex);
}

const QueryNode* QueryTree::getNode(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= nodes.size()) {
        return nullptr;
    }
    return &nodes[index];
}

// FIXME: make this an operator overload
void QueryTree::print() const {
    for (const auto& node : nodes) {
        std::cout << "Node " << node.getNodeIndex() << ": " << toString(node.getOperation());
        if (!node.getValue().empty()) {
            std::cout << " (" << node.getValue() << ")";
        }
        std::cout << std::endl;
    }
}

} // namespace queryTree
