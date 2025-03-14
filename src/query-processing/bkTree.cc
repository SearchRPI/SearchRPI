#include "../../include/query-processing/bkTree.h"
#include <algorithm>
#include <limits>

namespace bk {

BKTree::BKTree() : root(nullptr) {}

BKTree::BKTree(const Dictionary& dict) : root(nullptr) {
    for (const auto& word : dict) {
        insert(word);
    }
}

void BKTree::insert(const std::string& word) {
    if (!root) {
        root = std::make_unique<Node>(word);
        return;
    }
    Node* node = root.get();
    while (true) {
        int dist = editDistance(word, node->word);
        if (node->children.find(dist) == node->children.end()) {
            node->children[dist] = std::make_unique<Node>(word);
            break;
        }
        node = node->children[dist].get();
    }
}

std::string BKTree::findClosest(const std::string& query, int threshold) const {
    if (!root || query.empty()) return query;

    std::string bestMatch = query;
    int bestDistance = threshold + 1;

    search(root.get(), query, threshold, bestMatch, bestDistance);

    // If no valid match was found within the threshold, return the original query
    return (bestDistance <= threshold) ? bestMatch : query;
}

void BKTree::search(const Node* node, const std::string& query, int threshold,
                    std::string& bestMatch, int& bestDistance) const {
    if (!node) return;

    int dist = editDistance(query, node->word);
    if (dist < bestDistance) {
        bestDistance = dist;
        bestMatch = node->word;
    }

    for (const auto& [d, child] : node->children) {
        if (d >= dist - threshold && d <= dist + threshold) {
            search(child.get(), query, threshold, bestMatch, bestDistance);
        }
    }
}

int BKTree::editDistance(const std::string& a, const std::string& b) {
    size_t n = a.size(), m = b.size();
    std::vector<int> prev(m + 1, 0), curr(m + 1, 0);

    for (size_t j = 0; j <= m; ++j) prev[j] = j;

    for (size_t i = 1; i <= n; ++i) {
        curr[0] = i;
        for (size_t j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                curr[j] = prev[j - 1];
            } else {
                curr[j] = 1 + std::min({prev[j], curr[j - 1], prev[j - 1]});
            }
        }
        std::swap(prev, curr);
    }
    return prev[m];
}


} // namespace bk
