#pragma once

/**
 * @file  searcher.h
 * @brief Class for searching a database
 */

#include "DatabaseWrapper.h"
#include "types.h"

#include <vector>
#include <string>

namespace SearchRPI {

/**
 *  A Searcher object represents a querying session - most of the options for
 *  running a query can be set on it, and the query is run via Searcher::Search().
 */
class Searcher {
public:
    // Disable default constructor
    Searcher() = delete;

    /** 
     *  @param db The database to search.
     */
    Searcher(const DatabaseWrapper& db) : db(db) {}

    /** 
     *  @param db The database to search.
     *  @param query Query used to search.
     */
    Searcher(const DatabaseWrapper& db, const SearchRPI::Query query);

    // Destructor
    ~Searcher();

    /**
     *  @brief Search database using query and any other internal settings.
     *  @param max_items Maximum number of documents to return.
     *  @return Ordered list of documents that match query.
     *  TODO: FIX RETURN DATATYPE
     */
    // SearchRPI:ResultsDataType Search(unsigned int max_items);


private:
    DatabaseWrapper* db;
    std::vector<std::string> query; // TODO: Custom Class for Handling Processed Query
    
    // Configuration Settings Here as needed
    // double time_limit;
};

}