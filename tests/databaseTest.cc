#include "Database.h"
#include <gtest/gtest.h>
#include <filesystem>

const std::string TEST_DB_PATH = "./testdb";

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove_all(TEST_DB_PATH); // Clean up previous test data
        db = new Database(TEST_DB_PATH);
    }

    void TearDown() override {
        delete db;
        std::filesystem::remove_all(TEST_DB_PATH); // Clean up after tests
    }

    Database* db;
};

// Constructor Tests
TEST(DatabaseConstructorTest, ConstructorSuccess) {
    ASSERT_NO_THROW(Database db(TEST_DB_PATH));
}

TEST(DatabaseConstructorTest, ConstructorFailure) {
    ASSERT_THROW(Database("/invalid/path"), std::runtime_error);
}

// Add Tests
TEST_F(DatabaseTest, AddSuccess) {
    Data data = {10, 1};
    ASSERT_NO_THROW(db->add("key1", data));
}

TEST_F(DatabaseTest, AddDuplicateKeys) {
    Data data1 = {5, 2};
    Data data2 = {15, 3};
    ASSERT_NO_THROW(db->add("key1", data1));
    ASSERT_NO_THROW(db->add("key1", data2));
}

TEST_F(DatabaseTest, AddEmptyKey) {
    Data data = {10, 1};
    ASSERT_THROW(db->add("", data), std::runtime_error);
}

// Get Tests
TEST_F(DatabaseTest, GetSuccess) {
    Data data = {10, 1};
    db->add("key1", data);
    auto results = db->get("key1");
    ASSERT_EQ(results.size(), 1);
    ASSERT_EQ(results[0].priority, 10);
    ASSERT_EQ(results[0].docId, 1);
}

TEST_F(DatabaseTest, GetFailure) {
    ASSERT_THROW(db->get("nonexistent"), std::runtime_error);
}

TEST_F(DatabaseTest, GetMultipleValues) {
    Data data1 = {5, 2};
    Data data2 = {15, 3};
    db->add("key1", data1);
    db->add("key1", data2);
    auto results = db->get("key1");
    ASSERT_EQ(results.size(), 2);
}

TEST_F(DatabaseTest, GetLimitedValues) {
    Data data1 = {5, 2};
    Data data2 = {15, 3};
    Data data3 = {25, 4};
    db->add("key1", data1);
    db->add("key1", data2);
    db->add("key1", data3);
    auto results = db->get("key1", 2);
    ASSERT_EQ(results.size(), 2);
}

TEST_F(DatabaseTest, GetMoreThanAvailable) {
    Data data1 = {5, 2};
    db->add("key1", data1);
    auto results = db->get("key1", 5);
    ASSERT_EQ(results.size(), 1);
}

// Remove Tests
TEST_F(DatabaseTest, RemoveSuccess) {
    Data data = {10, 1};
    db->add("key1", data);
    ASSERT_NO_THROW(db->remove("key1"));
    ASSERT_THROW(db->get("key1"), std::runtime_error);
}

TEST_F(DatabaseTest, RemoveFailure) {
    ASSERT_THROW(db->remove("nonexistent"), std::runtime_error);
}

TEST_F(DatabaseTest, RemoveEmptyKey) {
    ASSERT_THROW(db->remove(""), std::runtime_error);
}

// Edge Cases
TEST_F(DatabaseTest, AddLargeNumberOfEntries) {
    for (int i = 0; i < 1000; ++i) {
        Data data = {i, i};
        db->add("key" + std::to_string(i), data);
    }
    for (int i = 0; i < 1000; ++i) {
        auto results = db->get("key" + std::to_string(i));
        ASSERT_EQ(results.size(), 1);
        ASSERT_EQ(results[0].priority, i);
        ASSERT_EQ(results[0].docId, i);
    }
}

TEST_F(DatabaseTest, RetrieveFromEmptyDatabase) {
    ASSERT_THROW(db->get("nonexistent"), std::runtime_error);
}

TEST_F(DatabaseTest, AddNullKey) {
    Data data = {10, 1};
    ASSERT_THROW(db->add("", data), std::runtime_error);
}
