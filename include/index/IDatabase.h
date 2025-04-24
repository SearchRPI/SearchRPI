#pragma once

#include <string>
#include <vector>
#include "../types.h"

using SearchRPI::Data;

/**
 * @brief Interface for Database operations
 */
class IDatabase {
public:
    IDatabase() = default;
    virtual ~IDatabase() = default;

    // Disable Copy Constructor/Assignment Operator
    IDatabase(const IDatabase&) = delete;
    IDatabase& operator=(IDatabase const&) = delete;

    /**
     * @brief Add data to database
     *
     * @param key Index to add data to.
     * @param data Data to be added.
     */
    virtual void add(const std::string& key, const Data& data) = 0;

    /**
     * @brief Remove all data for a specific key
     *
     * @param key Index to remove data from.
     */
    virtual void remove(const std::string& key) = 0;

    /**
     * @brief Retrieve the first value(s) for a given key
     *
     * @param key Index to retrieve from.
     * @return All entries at provided key.
     */
    virtual std::vector<Data> get(const std::string& key) = 0;

    /**
     * @brief Retrieve the first 'n' values for a given key
     *
     * @param key Index to retrieve from.
     * @param n Max number of values returned
     * @return At most the first 'n' entries
     */
    virtual std::vector<Data> get(const std::string& key, size_t n) = 0;

    /**
     * @brief Retrieve the number of documents containing a given term
     *
     * @param key Index to retrieve from.
     * @return Number of documents associated with that term
     */
    virtual unsigned int termDocCount(const std::string& key) = 0;

};
