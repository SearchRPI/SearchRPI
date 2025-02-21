#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockDatabase.h"
#include "search/searcher.h"
#include "search/query.h"
#include "search/weight.h"

#include <memory>
#include <vector>

using ::testing::_;
using ::testing::Return;

namespace Ranking {

class SearcherTest : public ::testing::Test {
protected:
    std::shared_ptr<MockDatabase> mockDB;
    Weight weightScheme;
    std::unique_ptr<Searcher> searcher;

    void SetUp() override {
        mockDB = std::make_shared<MockDatabase>();
        searcher = std::make_unique<Searcher>(mockDB, weightScheme);
    }
};

TEST_F(SearcherTest, SearchReturnsExpectedDocs) {
    // Prepare mock data.
    std::vector<Data> fakeData = {
        {10, 123},  // priority=10, docId=123
        {5, 456}
    };

    // Mock the DB call.
    EXPECT_CALL(*mockDB, get("foo", _))
        .Times(1)
        .WillOnce(Return(fakeData));

    Query query;
    query.addTerm("foo");

    unsigned int maxDocs = 10;
    MatchingDocs results = searcher->Search(query, maxDocs);
    std::vector<SearchResult> docs = results.get_all_results();

    ASSERT_EQ(docs.size(), 2u);
    EXPECT_EQ(docs[0].get_docid(), 123);
    EXPECT_EQ(docs[1].get_docid(), 456);
}

// Search Query with no Relevant Documents
TEST_F(SearcherTest, SearchWithNoResults) {
    EXPECT_CALL(*mockDB, get("nonexistent_term", _))
        .Times(1)
        .WillOnce(Return(std::vector<Data>{}));

    Query query;
    query.addTerm("nonexistent_term");

    MatchingDocs results = searcher->Search(query, 10);
    EXPECT_TRUE(results.get_all_results().empty());
}

// Test Max Docs Param
TEST_F(SearcherTest, SearchExceedsMaxDocs) {
    std::vector<Data> fakeData = {
        {10, 100}, {9, 101}, {8, 102}
    };
    EXPECT_CALL(*mockDB, get("foo", _))
        .Times(1)
        .WillOnce(Return(fakeData));

    Query query;
    query.addTerm("foo");

    unsigned int maxDocs = 2;
    MatchingDocs results = searcher->Search(query, maxDocs);
    std::vector<SearchResult> docs = results.get_all_results();

    ASSERT_EQ(docs.size(), 2u);
    EXPECT_EQ(docs[0].get_docid(), 100);
    EXPECT_EQ(docs[1].get_docid(), 101);
}

// Multiple Query Tokens
TEST_F(SearcherTest, SearchWithMultipleTerms) {
    std::vector<Data> fooData = {
        {10, 123}, {5, 456}
    };
    std::vector<Data> barData = {
        {7, 789}, {3, 111}
    };

    EXPECT_CALL(*mockDB, get("foo", _))
        .Times(1)
        .WillOnce(Return(fooData));
    EXPECT_CALL(*mockDB, get("bar", _))
        .Times(1)
        .WillOnce(Return(barData));

    Query query;
    query.addTerm("foo");
    query.addTerm("bar");

    MatchingDocs results = searcher->Search(query, 10);
    std::vector<SearchResult> docs = results.get_all_results();

    ASSERT_EQ(docs.size(), 4u);
    std::vector<unsigned int> docIds;
    for (auto& d : docs) {
        docIds.push_back(d.get_docid());
    }

    EXPECT_TRUE(std::find(docIds.begin(), docIds.end(), 123) != docIds.end());
    EXPECT_TRUE(std::find(docIds.begin(), docIds.end(), 456) != docIds.end());
    EXPECT_TRUE(std::find(docIds.begin(), docIds.end(), 789) != docIds.end());
    EXPECT_TRUE(std::find(docIds.begin(), docIds.end(), 111) != docIds.end());
}

// Empty Query
TEST_F(SearcherTest, SearchWithEmptyQuery) {
    // No DB calls expected.
    EXPECT_CALL(*mockDB, get(_, _)).Times(0);

    Query query; // empty

    MatchingDocs results = searcher->Search(query, 5);
    EXPECT_TRUE(results.get_all_results().empty());
}

} // namespace SearchRPI
