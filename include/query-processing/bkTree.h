#pragma once

/**
  ******************************************************************************
  * @file           : bkTree.h
  * @brief          : Data structure for fuzzy-finding strings.
  * @date           : 1/31/2025
  ******************************************************************************
  NOTE: This could fit in a utils folder
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

/**
 * @namespace bk
 * Namespace for BK-Tree based approximate string matching.
 */
namespace bk {
    using Dictionary = std::unordered_set<std::string>;

/**
 * @class BKTree
 * @brief A Burkhard-Keller Tree for approximate string matching using edit distance.
 */
class BKTree {
public:
    struct Node {
        std::string word;
        std::unordered_map<int, std::unique_ptr<Node>> children;
        explicit Node(const std::string& w) : word(w) {}
    };

    /**
     * @brief Constructs an empty BK-Tree.
     */
    BKTree();

    /**
     * @brief Constructs a BK-Tree from a given dictionary.
     * @param dict The dictionary of words to insert.
     */
    explicit BKTree(const Dictionary& dict);

    /**
     * @brief Tree root getter.
     * @return root of tree.
     */
    const Node* getRoot() const { return root.get(); }

    /**
     * @brief Inserts a word into the BK-Tree.
     * @param word The word to insert.
     */
    void insert(const std::string& word);

    /**
     * @brief Finds the closest word(s) to a given query within a certain edit distance.
     * @param query The input word.
     * @param threshold Maximum edit distance allowed.
     * @return The closest word found in the tree.
     */
    std::string findClosest(const std::string& query, int threshold = 2) const;

private:

    std::unique_ptr<Node> root;

    /**
     * @brief Recursively searches for the closest match in the BK-Tree.
     * @param node The current node being examined.
     * @param query The word to search for.
     * @param threshold The maximum allowed edit distance.
     * @param bestMatch The best match found so far.
     * @param bestDistance The minimum edit distance found so far.
     */
    void search(const Node* node, const std::string& query, int threshold,
                std::string& bestMatch, int& bestDistance) const;

    /**
     * @brief Computes the edit distance between two words.
     * @param a First word.
     * @param b Second word.
     * @return The Levenshtein edit distance.
     */
    static int editDistance(const std::string& a, const std::string& b);
};
} // namespace bk
