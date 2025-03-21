// #include <gtest/gtest.h>
// #include <string>
// #include <vector>
// #include "../src/query-processing/query.hpp"

// TEST(QueryTest, TokenizationWorksCorrectly) {
//     Query query("hello world test query");

//     // Tokenizes into words
//     auto tokens = query.tokenize();
//     ASSERT_EQ(tokens.size(), 4);
//     EXPECT_EQ(tokens[0], "hello");
//     EXPECT_EQ(tokens[1], "world");
//     EXPECT_EQ(tokens[2], "test");
//     EXPECT_EQ(tokens[3], "query");
// }

// // TEST_CASE("Query typo correction works correctly", "[Query]") {
// //     Query query("helo wrld");
// //
// //     SECTION("Corrects simple typos") {
// //         auto corrected = query.correctTypos();
// //         REQUIRE(corrected == "hello world");
// //     }
// // }
// //
// // TEST_CASE("Query expansion adds related terms", "[Query]") {
// //     Query query("fast car");
// //
// //     SECTION("Expands with synonyms") {
// //         auto expanded = query.expandQuery();
// //         REQUIRE_FALSE(expanded.empty());
// //         REQUIRE(std::find(expanded.begin(), expanded.end(), "quick vehicle") != expanded.end());
// //     }
// // }
// //
// // TEST_CASE("Query retrieves relevant document IDs", "[Query]") {
// //     Query query("example query");
// //
// //     SECTION("Returns non-empty list of document IDs") {
// //         auto doc_ids = query.getRelevantDocIDs();
// //         REQUIRE_FALSE(doc_ids.empty());
// //         REQUIRE(doc_ids[0] == 1);
// //     }
// // }
