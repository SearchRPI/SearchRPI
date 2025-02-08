#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockDatabase.h"
#include "search/searcher.h"
#include "search/query.h"
#include "search/weight.h"

using ::testing::_;
using ::testing::Return;

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
    Weight weightScheme;
    // Possibly configure weightScheme ...

    // 4. Construct the Searcher with the mock DB
    SearchRPI::Searcher searcher(&mockDb, weightScheme);

    // 5. Create a query. Let's say it’s just a single term "foo".
    Query query;
    query.someQueryKey = "foo"; // or however your Query is built

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
