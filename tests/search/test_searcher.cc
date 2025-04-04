#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockDatabase.h"
#include "search/searcher.h"
#include "search/weight.h"

#include <memory>
#include <vector>

using ::testing::_;
using ::testing::Return;

namespace Ranking {

class SearcherTest : public ::testing::Test {
protected:
    static query::Dictionary dict;
    static bk::BKTree* bkTree;
    static queryTree::TermDictionary termDictionary;

    std::shared_ptr<MockDatabase> mockDB;
    std::shared_ptr<Weight> bm25Weight = std::make_shared<BM25Weight>();
    std::unique_ptr<Searcher> searcher;

    void SetUp() override {
        mockDB = std::make_shared<MockDatabase>();
        searcher = std::make_unique<Searcher>(mockDB, bm25Weight);
    }

    static void SetUpTestSuite() {
        dict = {"hello", "world", "run", "happy", "children", "toy", "apple",
                "banana", "good", "ski", "buy", "foo", "bar", "nonexistentterm"};

        bkTree = new bk::BKTree(dict);

        termDictionary = {
            {"good ski", {}},
            {"ski resort", {}},
            {"fast runner", {}},
            {"mountain peak", {}}
        };
    }

    static void TearDownTestSuite() {
        delete bkTree;
    }
};

query::Dictionary SearcherTest::dict;
bk::BKTree* SearcherTest::bkTree = nullptr;
queryTree::TermDictionary SearcherTest::termDictionary;

TEST_F(SearcherTest, SearchReturnsExpectedDocs) {
    std::vector<Data> fakeData = {
        {10, 123}, {5, 456}
    };

    EXPECT_CALL(*mockDB, get("toy", _))
        .Times(1)
        .WillOnce(Return(fakeData));

    auto tree = query::processQuery("toy", dict, *bkTree, termDictionary);

    MatchingDocs results = searcher->Search(tree, 10);
    std::vector<SearchResult> docs = results.get_all_results();

    ASSERT_EQ(docs.size(), 2u);
    EXPECT_EQ(docs[0].get_docid(), 123);
    EXPECT_EQ(docs[1].get_docid(), 456);
}

// // Search Query with no Relevant Documents
TEST_F(SearcherTest, SearchWithNoResults) {
    EXPECT_CALL(*mockDB, get("nonexistentterm", _))
        .Times(1)
        .WillOnce(Return(std::vector<Data>{}));

    auto tree = query::processQuery("nonexistent_term", dict, *bkTree, termDictionary);
    MatchingDocs results = searcher->Search(tree, 10);

    EXPECT_TRUE(results.get_all_results().empty());
}

// // Test Max Docs Param
TEST_F(SearcherTest, SearchExceedsMaxDocs) {
    std::vector<Data> fakeData = {
        {10, 100}, {9, 101}, {8, 102}
    };

    EXPECT_CALL(*mockDB, get("foo", _))
        .Times(1)
        .WillOnce(Return(fakeData));

    auto tree = query::processQuery("foo", dict, *bkTree, termDictionary);
    MatchingDocs results = searcher->Search(tree, 2);
    std::vector<SearchResult> docs = results.get_all_results();

    ASSERT_EQ(docs.size(), 2u);
    EXPECT_EQ(docs[0].get_docid(), 100);
    EXPECT_EQ(docs[1].get_docid(), 101);
}


// // Multiple Query Tokens
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

    auto tree = query::processQuery("foo bar", dict, *bkTree, termDictionary);
    MatchingDocs results = searcher->Search(tree, 10);
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

// // Empty Query
TEST_F(SearcherTest, SearchWithEmptyQuery) {
    EXPECT_CALL(*mockDB, get(_, _)).Times(0);

    auto tree = query::processQuery("", dict, *bkTree, termDictionary);
    MatchingDocs results = searcher->Search(tree, 5);

    EXPECT_TRUE(results.get_all_results().empty());
}

// NOTE: Only uncomment if phrases are added
// TEST_F(SearcherTest, PhraseShouldScoreHigherThanSeparateTerms) {
//     std::vector<Data> goodData = {
//         {2, 101}, // in phrase doc
//         {5, 102}  // in single-term "good" doc
//     };
//     std::vector<Data> skiData = {
//         {3, 101}, // in phrase doc
//         {10, 103}  // in single-term "ski" doc
//     };
//     std::vector<Data> buyData = {
//         {3, 104}, // in phrase doc
//         {5, 103}  // in single-term "ski" doc
//     };

//     EXPECT_CALL(*mockDB, get("good", _)).Times(1).WillOnce(Return(goodData));
//     EXPECT_CALL(*mockDB, get("ski", _)).Times(1).WillOnce(Return(skiData));
//     EXPECT_CALL(*mockDB, get("buy", _)).Times(1).WillOnce(Return(buyData));

//     std::string query = "What are good skis to buy?";
//     auto tree = query::processQuery(query, dict, *bkTree, termDictionary);
//     std::cout << "Tree: \n" << tree << "\n" << std::endl;

//     MatchingDocs results = searcher->Search(tree, 10);
//     std::vector<SearchResult> docs = results.get_all_results();

//     ASSERT_EQ(docs.size(), 4u);

//     // Print rankings for debug
//     for (const auto& result : docs) {
//         std::cout << "DocID: " << result.get_docid()
//                   << ", Score: " << result.get_weight() << std::endl;
//     }

//     // If phrase logic is correctly prioritized, doc 101 should be ranked highest
//     EXPECT_EQ(docs[0].get_docid(), 101);
// }

} // namespace SearchRPI
