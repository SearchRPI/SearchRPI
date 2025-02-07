#pragma once

#include <string>
#include <vector>

struct Data;

/**
 * @brief Interface for Database operations
 *
 * This allows mocking and dependency injection in tests.
 */
class IDatabase {
public:
    virtual ~IDatabase() = default;

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
};
