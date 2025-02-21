#pragma once

/**
 * @file  query.h
 * @brief Query Class
*/

#include <string>
#include <vector>

namespace SearchRPI {

// Class representing a query
class Query {
public:

    Query() {}

    // Temporary functions while implementation is not concrete
    std::vector<std::string> terms() const { return query; }
    void addTerm(std::string term) { query.push_back(term); }

private:
    std::vector<std::string> query;

};

}