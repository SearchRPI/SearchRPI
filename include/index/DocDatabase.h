#pragma once

/**
 * @file  DocDatabase.h
 * @brief Document DB wrapper
 */

#include "index/IDocDatabase.h"
#include "types.h"

#include <lmdb.h>

#include <memory>
#include <string>
#include <set>
#include <sstream>
#include <cstring>

class DocDatabase : public IDocDatabase {
public:
    DocDatabase() { DocDatabase("../docdb"); }
    explicit DocDatabase(const std::string& dbPath);
    ~DocDatabase();

    /**
     * @param id Document ID
     * @returns Whether corups contains a document with that ID
     */
    bool contains(SearchRPI::docid id) const;

    /**
     * @param url URL of Site (e.g. sis.rpi.edu) (strip https://)
     * @returns Whether any document is located at that url
     */
    bool contains(const std::string& url) const;

    /**
     * @param url Page URL
     * @param title Page Title (Shown in search results)
     * @param words Text of the page // TODO: Update format to include punctuation & support field weighting
     * @returns ID Generated for Added Document
     */
    SearchRPI::docid addDoc(const std::string& url, const std::string& title, std::vector<std::string> words);

    /**
     * @param url Page URL
     * @returns ID associated with URL
     */
    SearchRPI::docid getDocId(const std::string& url) const;

    /**
     * @param id Document ID
     * @returns A set of all words in the document
     */
    std::set<std::string> getWords(SearchRPI::docid id) const;

    /**
     * @param id Document ID
     * @returns Whether document existed and was removed (false if doc didn't exist)
     */
    bool remove(SearchRPI::docid id);

private:
    // LMDB environment and database handles.
    MDB_env* env_;
    MDB_dbi dbi_meta_;
    MDB_dbi dbi_docs_;
    MDB_dbi dbi_urls_;
    
    // Helper: simple serialization of a document record.
    // Format: <url>\n<title>\n<word1,word2,...>
    std::string serializeDoc(const std::string& url, const std::string& title, const std::vector<std::string>& words);
    
    // Helper: deserialize a document record into its parts.
    void deserializeDoc(const std::string& data, std::string& url, std::string& title, std::vector<std::string>& words) const;
    
    // Helper: convert a document ID to a string.
    std::string docidToStr(SearchRPI::docid id) const { return std::to_string(id); }

};