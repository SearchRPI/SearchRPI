#pragma once

/**
 * @file  searcher.h
 * @brief Class for searching a database
*/

#include "types.h"

#include <vector>
#include <string>

namespace SearchRPI {

/** Querying session.
 *
 *  An Enquire object represents a querying session - most of the options for
 *  running a query can be set on it, and the query is run via
 *  Enquire::get_mset().
 */
class Searcher {
public:
    Searcher(); // Temporary, remove later

    /** 
     *  @param db The database to search.
     */
    // Searcher(const SearchRPI::Database& db);

    /** 
     *  @param db The database to search.
     *  @param query Query used to search.
     */
    // Searcher(const SearchRPI::Database& db, const SearchRPI::Query query);

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
    // SearchRPI::Database db;

    // TODO: Custom Class for Handling Processed Query
    std::vector<std::string> query; 


};

}