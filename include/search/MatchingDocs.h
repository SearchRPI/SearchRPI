#pragma once

/**
 * @file  MatchingDocs.h
 * @brief Class containing ordered search results for a query
*/

#include "types.h"
#include "SearchResult.h"

#include <vector>
#include <algorithm>

namespace SearchRPI {

/**
 * @class MatchingDocs
 * @brief Represents a collection of ordered search results for a query.
 */
class MatchingDocs {
public:
    MatchingDocs() {}

    // Temporary Functions
    void add_result(const SearchResult& sr) { results.push_back(sr); }
    void sort_by_score_desc() {
        std::sort(results.begin(), results.end(),
            [](const SearchResult& a, const SearchResult& b) {
                return a.get_weight() > b.get_weight();
            }
        );
    }
    const std::vector<SearchResult>& get_all_results() const { return results; }

    // Returns the number of search results in the collection.
    unsigned int size() const { return (unsigned int) results.size(); }
    
    // Checks if size of collection is zero.
    bool empty() const { return size() == 0; }

private:
    std::vector<SearchResult> results;
};

}