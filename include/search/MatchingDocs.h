#pragma once

/**
 * @file  MatchingDocs.h
 * @brief Class containing ordered search results for a query
*/

#include "types.h"
#include "SearchResult.h"

#include <vector>

namespace SearchRPI {

class MatchingDocs {
public:
    MatchingDocs();

    unsigned int size() const { return results.size(); }
    
    bool empty() const { return size() == 0; }


private:
    std::vector<SearchResult> results;

};

}