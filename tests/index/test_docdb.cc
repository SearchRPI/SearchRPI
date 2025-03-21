#include <gtest/gtest.h>

#include "index/DocDatabase.h"

#include <chrono>
#include <filesystem>
#include <memory>
#include <set>
#include <string>
#include <vector>

class DocDBTest : public ::testing::Test {
protected:
    std::unique_ptr<DocDatabase> docdb;
    std::string db_path;

    void SetUp() override {
        // Create a temporary directory for the LMDB environment.
        db_path = "./temp_docdb_test";
        std::filesystem::create_directory(db_path);
        docdb = std::make_unique<DocDatabase>(db_path);
    }

    void TearDown() override {
        // Release the DocDatabase and remove the temporary directory.
        docdb.reset();
        std::filesystem::remove_all(db_path);
    }
};

// Test adding a single document and retrieving its words.
TEST_F(DocDBTest, TestAddAndRetrieveDocument) {
    std::string url = "example.com/page";
    std::string title = "Test Page";
    std::vector<std::string> words = {"hello", "world", "test"};

    SearchRPI::docid docId = docdb->addDoc(url, title, words);
    SearchRPI::docid retrievedId = docdb->getDocId(url);
    std::set<std::string> retrievedWords = docdb->getWords(docId);
    EXPECT_EQ(docId, retrievedId);

    std::set<std::string> wordsSet(words.begin(), words.end());
    EXPECT_EQ(wordsSet, retrievedWords);
}

// Test adding a document and checking if it is contained, before and after removing.
TEST_F(DocDBTest, TestContainsAndRemoveDocument) {
    std::string url = "example.com/anotherpage";
    std::string title = "Another Test Page";
    std::vector<std::string> words = {"alpha", "beta", "gamma"};
    EXPECT_FALSE(docdb->contains(url));

    SearchRPI::docid docId = docdb->addDoc(url, title, words);
    EXPECT_TRUE(docdb->contains(url));
    EXPECT_TRUE(docdb->contains(docId));

    bool removed = docdb->remove(docId);
    EXPECT_TRUE(removed);
    EXPECT_FALSE(docdb->contains(url));
    EXPECT_FALSE(docdb->contains(docId));
}

// Test that adding a duplicate document returns the same document id.
TEST_F(DocDBTest, TestDuplicateDocument) {
    std::string url = "example.com/duplicate";
    std::string title = "Duplicate Test Page";
    std::vector<std::string> words = {"dup", "test"};

    SearchRPI::docid firstId = docdb->addDoc(url, title, words);
    SearchRPI::docid secondId = docdb->addDoc(url, title, words);

    EXPECT_EQ(firstId, secondId);
}

// Test that getting a document id for a non-existent URL throws an exception.
TEST_F(DocDBTest, TestGetNonExistentDocument) {
    std::string nonExistentUrl = "nonexistent.com/page";
    EXPECT_THROW({
        try {
            docdb->getDocId(nonExistentUrl);
        } catch (const std::runtime_error& e) {
            throw;
        }
    }, std::runtime_error);
}

// Test that removing a non-existent document returns false.
TEST_F(DocDBTest, TestRemoveNonExistentDocument) {
    // Use a docid that was never added.
    SearchRPI::docid fakeId = 9999;
    EXPECT_FALSE(docdb->remove(fakeId));
}

// Test adding multiple documents and verify that they all exist with unique IDs.
TEST_F(DocDBTest, TestAddMultipleDocuments) {
    const int numDocs = 100;
    std::vector<SearchRPI::docid> docIds;
    for (int i = 0; i < numDocs; i++) {
        std::string url = "example.com/page" + std::to_string(i);
        std::string title = "Page " + std::to_string(i);
        std::vector<std::string> words = {"word" + std::to_string(i), "test"};
        SearchRPI::docid id = docdb->addDoc(url, title, words);
        docIds.push_back(id);
        EXPECT_TRUE(docdb->contains(id));
    }
    std::set<SearchRPI::docid> uniqueIds(docIds.begin(), docIds.end());
    EXPECT_EQ(uniqueIds.size(), static_cast<size_t>(numDocs));
}

// Performance test: measure how long it takes to add a large number of documents.
// Note: The performance threshold here (per document) might need adjustment
TEST_F(DocDBTest, PerformanceTest_AddDocuments) {
    const int numDocs = 1000;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numDocs; i++) {
        std::string url = "perf.example.com/page" + std::to_string(i);
        std::string title = "Performance Page " + std::to_string(i);
        std::vector<std::string> words = {"performance", "test", std::to_string(i)};
        docdb->addDoc(url, title, words);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double timePerDoc = elapsed.count() / numDocs;
    
    std::cout << "PerformanceTest: Added " << numDocs << " documents in " 
              << elapsed.count() << " seconds (" << timePerDoc 
              << " sec per document)" << std::endl;
    
    EXPECT_LT(timePerDoc, 0.01);
}

// Performance test: measure how long it takes to retrieve a large number of documents.
// This test preloads the database with a number of documents and then retrieves them one-by-one.
TEST_F(DocDBTest, PerformanceTest_RetrieveDocuments) {
    const int numDocs = 1000;
    std::vector<SearchRPI::docid> docIds;
    
    // Preload the database with documents.
    for (int i = 0; i < numDocs; i++) {
        std::string url = "retrieve.example.com/page" + std::to_string(i);
        std::string title = "Retrieve Page " + std::to_string(i);
        std::vector<std::string> words = {"retrieve", "test", std::to_string(i)};
        docIds.push_back(docdb->addDoc(url, title, words));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (SearchRPI::docid docId : docIds) {
        volatile auto words = docdb->getWords(docId);
        // The volatile keyword prevents compiler optimizations that might remove the call.
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double timePerDoc = elapsed.count() / numDocs;
    
    std::cout << "PerformanceTest: Retrieved " << numDocs 
              << " documents in " << elapsed.count() 
              << " seconds (" << timePerDoc 
              << " sec per document)" << std::endl;

    EXPECT_LT(timePerDoc, 0.001);
}
