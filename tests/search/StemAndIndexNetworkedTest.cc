#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>

#include "../mock/MockDatabase.h"  // Assuming your MockDatabase is here
#include "search/StemAndIndexNetworked.cc"  // Access to handle_connection

using ::testing::_;
using ::testing::Invoke;
using ::testing::Exactly;

TEST(StemAndIndexNetworkedTest, HandleConnectionProcessesStemmingAndTagsCorrectly) {
    int sv[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, sv), 0);

    MockDatabase mock_db;
    std::unordered_map<std::string, std::vector<Data>> in_memory_db;

    // Simulate add behavior (store in in-memory map)
    EXPECT_CALL(mock_db, add(_, _))
        .Times(Exactly(2))
        .WillRepeatedly(Invoke([&in_memory_db](const std::string& key, const Data& data) {
            in_memory_db[key].push_back(data);
        }));

    // Provide basic get behavior if needed later
    EXPECT_CALL(mock_db, get(_))
        .WillRepeatedly(Invoke([&in_memory_db](const std::string& key) -> std::vector<Data> {
            if (in_memory_db.count(key) == 0) throw std::runtime_error("Key not found");
            return in_memory_db[key];
        }));

    EXPECT_CALL(mock_db, get(_, _))
        .WillRepeatedly(Invoke([&in_memory_db](const std::string& key, size_t n) -> std::vector<Data> {
            if (in_memory_db.count(key) == 0) throw std::runtime_error("Key not found");
            auto& all = in_memory_db[key];
            return std::vector<Data>(all.begin(), all.begin() + std::min(n, all.size()));
        }));

    // Prepare test JSON
    std::string test_json = R"({
        "url": "https://test.com/page",
        "words": {
            "proofs": {"count": 2, "tag": "h1"},
            "math": {"count": 3, "tag": "p"}
        }
    })"
    "\n";  // Raw string correctly terminated

    write(sv[0], test_json.c_str(), test_json.size());

    handle_connection_for_tests(sv[1], mock_db);

    close(sv[0]);
    close(sv[1]);

    // Validate in-memory "DB" content after connection
    ASSERT_TRUE(in_memory_db.find("proof") != in_memory_db.end());
    ASSERT_TRUE(in_memory_db.find("math") != in_memory_db.end());

    auto proof_entries = in_memory_db["proof"];
    auto math_entries = in_memory_db["math"];

    ASSERT_EQ(proof_entries[0].priority, 80);  // h1 tag -> 80 priority
    ASSERT_EQ(math_entries[0].priority, 40);   // p tag -> 40 priority
}
