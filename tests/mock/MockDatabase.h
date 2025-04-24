#include <gmock/gmock.h>
#include "index/IDatabase.h"

class MockDatabase : public IDatabase {
public:
    MOCK_METHOD(void, add, (const std::string& key, const Data& data), (override));
    MOCK_METHOD(std::vector<Data>, get, (const std::string& key), (override));
    MOCK_METHOD(std::vector<Data>, get, (const std::string& key, size_t n), (override));
    MOCK_METHOD(void, remove, (const std::string& key), (override));
    MOCK_METHOD(unsigned int, termDocCount, (const std::string& key), (override));
};
