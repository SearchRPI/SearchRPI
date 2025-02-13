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
    // 1. Create a mock database object
    MockDatabase mockDb;

    // 2. Set up expectations: what calls do we expect on the mock?
    //    Suppose we expect db_->get("foo") to be called exactly once
    //    and return some data.
    std::vector<Data> fakeData = {
        {10, 123}, // priority=10, docId=123
        {5, 456},
    };
    EXPECT_CALL(mockDb, get("foo"))
        .Times(1)
        .WillOnce(Return(fakeData));

    // 3. Construct a Weight object (stub or real, up to you)
    Weight weightScheme();

    // 4. Construct the Searcher with the mock DB
    Searcher searcher(std::make_shared<IDatabase>(mockDb), weightScheme);

    // 5. Create Query
    Query query;
    query.addTerm("foo");

    // 6. Call the method under test
    unsigned int maxDocs = 10;
    MatchingDocs results = searcher.Search(query, maxDocs);

    // 7. Verify: did we get the correct results?
    ASSERT_EQ(results.size(), 2u);
    EXPECT_EQ(results[0].docId, 123);
    EXPECT_EQ(results[1].docId, 456);

    // The mock expectations (get("foo")) are verified automatically 
    // when the test ends.
}

}