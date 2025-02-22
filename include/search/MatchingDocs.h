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

    // Returns the number of search results in the collection.
    unsigned int size() const { return results.size(); }
    
    // Checks if size of collection is zero.
    bool empty() const { return size() == 0; }

private:
    std::vector<SearchResult> results;
};

}