#pragma once

#include "IDatabase.h"
#include <gmock/gmock.h>

/**
 * @brief Mock class for IDatabase that uses Google Mock macros.
 */
class MockDatabase : public IDatabase {
public:
    MOCK_METHOD(void, add, (const std::string& key, const Data& data), (override));
    MOCK_METHOD(void, remove, (const std::string& key), (override));
    MOCK_METHOD(std::vector<Data>, get, (const std::string& key), (override));
    MOCK_METHOD(std::vector<Data>, get, (const std::string& key, size_t n), (override));
};
