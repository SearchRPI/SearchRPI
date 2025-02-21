#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockDatabase.h"
#include "search/searcher.h"
#include "search/query.h"
#include "search/weight.h"

#include <memory>

using ::testing::_;
using ::testing::Return;

namespace SearchRPI {

TEST(SearcherTest, SearchReturnsExpectedDocs) {
    std::shared_ptr<MockDatabase> mockDB = std::make_shared<MockDatabase>();

    std::vector<Data> fakeData = {
        {10, 123}, // priority=10, docId=123
        {5, 456},
    };
    EXPECT_CALL(*mockDB, get("foo", _))
        .Times(1)
        .WillOnce(Return(fakeData));

    Weight weightScheme;
    Searcher searcher(mockDB, weightScheme);

    // TEMPORARY, until Query behavior is figured out
    Query query;
    query.addTerm("foo");

    unsigned int maxDocs = 10;
    MatchingDocs results = searcher.Search(query, maxDocs);

    // TEMPORARY
    std::vector<SearchResult> docs = results.get_all_results(); 
    ASSERT_EQ(docs.size(), 2u);
    EXPECT_EQ(docs[0].get_docid(), 123);
    EXPECT_EQ(docs[1].get_docid(), 456);
}

}