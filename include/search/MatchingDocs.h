#pragma once

/**
 * @file  MatchingDocs.h
 * @brief Class containing ordered search results for a query
*/

#include "types.h"
#include "SearchResult.h"

#include <vector>

namespace SearchRPI {

/**
 * @class MatchingDocs
 * @brief Represents a collection of ordered search results for a query.
 */
class MatchingDocs {
public:
    MatchingDocs();

    // Add a single search result
    void add_result(const SearchResult& sr) {
        results.push_back(sr);
    }

    // Sort search results by descending weight (score)
    void sort_by_score_desc() {
        std::sort(results.begin(), results.end(),
            [](const SearchResult& a, const SearchResult& b) {
                return a.get_weight() > b.get_weight();
            }
        );
    }

    // Returns the number of search results in the collection.
    unsigned int size() const { return (unsigned int) results.size(); }
    
    // Checks if size of collection is zero.
    bool empty() const { return size() == 0; }

private:
    std::vector<SearchResult> results;
};

}