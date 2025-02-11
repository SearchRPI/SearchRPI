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

    Query();

    // Temporary function while implementation is not concrete
    std::vector<std::string> terms() const { return query; }

private:
    std::vector<std::string> query;

};

}