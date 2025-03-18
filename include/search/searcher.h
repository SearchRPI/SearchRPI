#pragma once

/**
 * @file  searcher.h
 * @brief Class for searching a database
 */

#include "types.h"
#include "index/IDatabase.h"
#include "query.h"
#include "search/weight.h"
#include "search/MatchingDocs.h"

#include <vector>
#include <string>
#include <memory>

namespace Ranking {

/**
 *  A Searcher object represents a querying session - most of the options for
 *  running a query can be set on it, and the query is run via Searcher::Search().
 */
class Searcher {
public:
    // Disable default constructor
    Searcher() = delete;

    /** 
     * @param db The database to search.
     * @param weight The weighting scheme to use for ranking results.
     */
    Searcher(std::shared_ptr<IDatabase> db, std::shared_ptr<Weight> weight) 
            : db(db), weight_scheme(weight) {}

    /**
     *  @brief Search database using query and any other internal settings.
     *  @param query Query used to search database.
     *  @param max_items Maximum number of documents to return.
     *  @return Ordered list of documents that match query.
     */
    MatchingDocs Search(const Query& query, unsigned int max_items);


private:
    std::shared_ptr<IDatabase> db;
    std::shared_ptr<Weight> weight_scheme;
    
    // Configuration Settings Here as needed
    // double time_limit;
};

}