#include "Database.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <unordered_map>
#include <algorithm>

const std::string TEST_DB_PATH = "./testdb";

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (std::filesystem::exists(TEST_DB_PATH)) {
            std::filesystem::remove_all(TEST_DB_PATH); // Remove all existing contents
        } else {
            std::filesystem::create_directories(TEST_DB_PATH); // Create the directory
        }
        db = new Database(TEST_DB_PATH);
    }

    void TearDown() override {
        delete db;
        std::filesystem::remove_all(TEST_DB_PATH); // Clean up after tests
    }

    Database* db;
};

std::string GetTestFilePath(const std::string& relative_path) {
    std::filesystem::path base = __FILE__;
    base = base.parent_path();
    return (base / relative_path).string();
}

// Constructor Tests
TEST(DatabaseConstructorTest, ConstructorSuccess) {
    std::string path = GetTestFilePath("testdb");
    std::filesystem::create_directory(path);  // Make sure directory exists
    ASSERT_NO_THROW(Database db(path));
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

TEST_F(DatabaseTest, IndexMultipleWordsWithMultipleDocs) {
    std::unordered_map<std::string, std::vector<Data>> index = {
        { "c++",     { {100, 1}, {90, 2}, {80, 3} } },
        { "search",  { {70, 4}, {60, 5}, {50, 6}, {40, 7} } },
        { "engine",  { {30, 8}, {20, 9}, {10, 10} } }
    };

    // Add all entries to the DB
    for (const auto& [word, docs] : index) {
        for (const auto& d : docs) {
            db->add(word, d);
        }
    }

    // Retrieve and check
    for (const auto& [word, expected_docs] : index) {
        auto results = db->get(word);

        // Check size matches
        ASSERT_EQ(results.size(), expected_docs.size());

        // Check all expected docIds are found
        std::vector<int> found_ids;
        for (const auto& d : results) {
            found_ids.push_back(d.docId);
        }

        for (const auto& expected : expected_docs) {
            ASSERT_TRUE(std::find(found_ids.begin(), found_ids.end(), expected.docId) != found_ids.end());
        }

        // Check descending order by priority
        for (size_t i = 1; i < results.size(); ++i) {
            ASSERT_GE(results[i - 1].priority, results[i].priority);
        }
    }
}

TEST_F(DatabaseTest, TermDocCountSuccess) {
    Data data1 = {10, 1};
    Data data2 = {20, 2};
    Data data3 = {30, 3};
    db->add("term", data1);
    db->add("term", data2);
    db->add("term", data3);
    unsigned int count = db->termDocCount("term");
    ASSERT_EQ(count, 3);
}

TEST_F(DatabaseTest, TermDocCountNonexistentKey) {
    unsigned int count = db->termDocCount("nonexistent");
    ASSERT_EQ(count, 0);
}
