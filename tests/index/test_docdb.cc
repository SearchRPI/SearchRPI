#include <gtest/gtest.h>

#include "DocDatabase.h"

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <filesystem>

using ::testing::_;
using ::testing::Return;

namespace Index {

class DocDBTest : public ::testing::Test {
protected:
    std::unique_ptr<DocDatabase> docdb;

    void SetUp() override {
        // Create a temporary directory for the LMDB environment.
        db_path_ = "./temp_docdb_test";
        std::filesystem::create_directory(db_path_);
        doc_db_ = std::make_unique<DocDatabase>(db_path_);
    }

    void TearDown() override {
        // Release the DocDatabase and remove the temporary directory.
        doc_db_.reset();
        std::filesystem::remove_all(db_path_);
    }
};

TEST_F(DocDBTest, TestAddAndRetrieveDocument) {
    std::string url = "example.com/page";
    std::string title = "Test Page";
    std::vector<std::string> words = {"hello", "world", "test"};

    SearchRPI::docid docId = doc_db_->addDoc(url, title, words);
    SearchRPI::docid retrievedId = doc_db_->getDocId(url);
    std::set<std::string> retrievedWords = doc_db_->getWords(docId);
    EXPECT_EQ(docId, retrievedId);

    std::set<std::string> wordsSet(words.begin(), words.end());
    EXPECT_EQ(wordsSet, retrievedWords);
}

TEST_F(DocDBTest, TestContainsAndRemoveDocument) {
    std::string url = "example.com/anotherpage";
    std::string title = "Another Test Page";
    std::vector<std::string> words = {"alpha", "beta", "gamma"};
    EXPECT_FALSE(doc_db_->contains(url));

    SearchRPI::docid docId = doc_db_->addDoc(url, title, words);
    EXPECT_TRUE(doc_db_->contains(url));
    EXPECT_TRUE(doc_db_->contains(docId));

    bool removed = doc_db_->remove(docId);
    EXPECT_TRUE(doc_db_->remove(docId));
    EXPECT_FALSE(doc_db_->contains(url));
    EXPECT_FALSE(doc_db_->contains(docId));
}

}