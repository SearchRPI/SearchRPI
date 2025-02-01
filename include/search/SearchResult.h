#pragma once

/**
 * @file  Result.h
 * @brief Class containing a search result
*/

#include "types.h"

#include <string>

namespace SearchRPI {

// Represents a single search result
class SearchResult {
public:
    // Disable default constructor
    SearchResult() = delete;

    SearchResult(docid id) : id(id) {}

    // Get document associated with this result
    docid docID() const { return id; }

protected:
    docid id;
    std::string page_name; // NOTE: Just site name/domain for now, do something better later
    std::string snippet;
    std::string url;

    // TODO: Add favicon

};

}