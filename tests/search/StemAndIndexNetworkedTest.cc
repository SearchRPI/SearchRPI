#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sys/socket.h>
#include <unistd.h>

#include "DatabaseWrapper.h"
#include "StemAndIndexNetworked.cc"  // Direct access to handle_connection()

using ::testing::_;
using ::testing::Exactly;
using ::testing::Invoke;

class MockDatabase : public Database {
public:
    MockDatabase() : Database("./testdb") {}

    MOCK_METHOD(void, add, (const std::string& key, const Data& data), (override));
    MOCK_METHOD(void, remove, (const std::string& key), (override));
    MOCK_METHOD(std::vector<Data>, get, (const std::string& key), (override));
    MOCK_METHOD(std::vector<Data>, get, (const std::string& key, size_t n), (override));
};

TEST(StemAndIndexNetworkedTest, HandleConnectionProcessesStemmingAndTagsCorrectly) {
    int sv[2];
    ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, sv), 0);

    MockDatabase mock_db;

    // Set expectations
    EXPECT_CALL(mock_db, add(_, _))
        .Times(Exactly(2))
        .WillRepeatedly(Invoke([](const std::string& key, const Data& data) {
            EXPECT_FALSE(key.empty());
            EXPECT_GT(data.priority, 0);
            EXPECT_GT(data.docId, 0);
        }));

    // Prepare JSON input
    std::string test_json = R"({
        "url": "https://test.com/page",
        "words": {
            "proofs": {"count": 2, "tag": "h1"},
            "math": {"count": 3, "tag": "p"}
        }
    })\n";

    write(sv[0], test_json.c_str(), test_json.size());

    handle_connection(sv[1], mock_db);

    close(sv[0]);
    close(sv[1]);
}
