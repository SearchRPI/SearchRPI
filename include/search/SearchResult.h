#pragma once

/**
 * @file  Result.h
 * @brief Class containing a search result
*/

#include "types.h"

#include <string>

namespace Ranking {

// Represents a single search result
// NOTE: Could be struct right now, may swap later
class SearchResult {
public:
    // Disable default constructor
    // SearchResult() = delete;

    // Disable copy constructor(s)
    // SearchResult(const SearchResult&) = delete;
    // SearchResult& operator=(const SearchResult&) = delete;

    SearchResult(double weight, SearchRPI::docid id) : weight(weight), id(id) {}

    // Get document associated with this search result
    SearchRPI::docid get_docid() const { return id; }

    // Get the weight (score) associated with this search result.
    double get_weight() const { return weight; }

protected:
    double weight;
    SearchRPI::docid id;
};

}