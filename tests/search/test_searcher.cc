#include <gtest/gtest.h>

#include "../../include/search/searcher.h"

TEST(QueryTest, TokenizationWorksCorrectly) {
    Database db;
    EXPECT_CALL(db, get("key"))
        .WillOnce(Return("duh"));

    results = Searcher.Search("to be or not be");
}

// test_searcher.cpp

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../include/search/searcher.h"

using ::testing::Return;

// Example mock class matching the Database interface
class MockDatabase : public IDatabase {
public:
    MOCK_METHOD(std::string, get, (const std::string& key), (override));
};

class SearchTest : public ::testing::Test {
protected:
    void SetUp() override {
        db = std::make_unique<Database>();
        searcher = std::make_unique<Searcher>(std::move(mock_db));
    }

    std::unique_ptr<Database> db;
    std::unique_ptr<Search> searcher;
};

TEST_F(SearchTest, MockDatabaseCall) {
    EXPECT_CALL(*mock_db, Query("SELECT * FROM users"))
        .WillOnce(::testing::Return("mocked response"));

    std::string result = search->PerformSearch("SELECT * FROM users");
    EXPECT_EQ(result, "mocked response");
}

// Standard test runner
int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
