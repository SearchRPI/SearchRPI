#include "index/DocDatabase.h"

#include <iostream>

DocDatabase::DocDatabase(const std::string& dbPath) {
    int rc = mdb_env_create(&env_);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("Failed to create LMDB environment");
    
    // We use 3 named databases (meta, docs, urls)
    rc = mdb_env_set_maxdbs(env_, 3);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("Failed to set maxdbs");
    
    rc = mdb_env_open(env_, dbPath.c_str(), 0, 0664);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("Failed to open LMDB environment");
    
    MDB_txn* txn;
    rc = mdb_txn_begin(env_, nullptr, 0, &txn);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("Failed to begin LMDB transaction");
    
    // Open/create the meta, docs, and urls databases.
    rc = mdb_dbi_open(txn, "meta", MDB_CREATE, &dbi_meta_);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("Failed to open meta database");
    }
    rc = mdb_dbi_open(txn, "docs", MDB_CREATE, &dbi_docs_);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("Failed to open docs database");
    }
    rc = mdb_dbi_open(txn, "urls", MDB_CREATE, &dbi_urls_);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("Failed to open urls database");
    }
    
    // Initialize next_docid in meta DB if it does not exist.
    MDB_val key, data;
    const char* nextKey = "next_docid";
    key.mv_size = std::strlen(nextKey);
    key.mv_data = (void*)nextKey;
    rc = mdb_get(txn, dbi_meta_, &key, &data);
    if (rc == MDB_NOTFOUND) {
        uint64_t next = 1;
        data.mv_size = sizeof(next);
        data.mv_data = &next;
        rc = mdb_put(txn, dbi_meta_, &key, &data, 0);
        if (rc != MDB_SUCCESS) {
            mdb_txn_abort(txn);
            throw std::runtime_error("Failed to initialize next_docid");
        }
    }
    mdb_txn_commit(txn);
}

DocDatabase::~DocDatabase() {
    mdb_dbi_close(env_, dbi_meta_);
    mdb_dbi_close(env_, dbi_docs_);
    mdb_dbi_close(env_, dbi_urls_);
    mdb_env_close(env_);
}

bool DocDatabase::contains(SearchRPI::docid id) const {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc != MDB_SUCCESS)
        return false;
    
    std::string keyStr = docidToStr(id);
    MDB_val key, data;
    key.mv_size = keyStr.size();
    key.mv_data = (void*)keyStr.data();
    
    rc = mdb_get(txn, dbi_docs_, &key, &data);
    mdb_txn_commit(txn);
    
    return (rc == MDB_SUCCESS);
}

bool DocDatabase::contains(const std::string& url) const {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc != MDB_SUCCESS)
        return false;
    
    MDB_val key, data;
    key.mv_size = url.size();
    key.mv_data = (void*)url.data();
    
    rc = mdb_get(txn, dbi_urls_, &key, &data);
    mdb_txn_commit(txn);
    
    return (rc == MDB_SUCCESS);
}

SearchRPI::docid DocDatabase::addDoc(const std::string& url, const std::string& title, std::vector<std::string> words) {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, 0, &txn);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("Failed to begin transaction in addDoc");
    
    // First, check if a document with the same URL already exists.
    MDB_val urlKey, urlData;
    urlKey.mv_size = url.size();
    urlKey.mv_data = (void*)url.data();
    rc = mdb_get(txn, dbi_urls_, &urlKey, &urlData);
    if (rc == MDB_SUCCESS) {
        // URL exists – return the existing document id.
        std::string docidStr((char*)urlData.mv_data, urlData.mv_size);
        uint64_t existingDocId = std::stoull(docidStr);
        mdb_txn_commit(txn);
        return existingDocId;
    }
    
    // Retrieve the next_docid from the meta DB.
    const char* nextKey = "next_docid";
    MDB_val metaKey, metaData;
    metaKey.mv_size = std::strlen(nextKey);
    metaKey.mv_data = (void*)nextKey;
    rc = mdb_get(txn, dbi_meta_, &metaKey, &metaData);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("Failed to get next_docid");
    }
    uint64_t docId;
    std::memcpy(&docId, metaData.mv_data, sizeof(docId));
    
    // Serialize the document record.
    std::string docRecord = serializeDoc(url, title, words);
    
    // Insert the document record into the docs DB.
    std::string docKeyStr = docidToStr(docId);
    MDB_val docKey, docValue;
    docKey.mv_size = docKeyStr.size();
    docKey.mv_data = (void*)docKeyStr.data();
    docValue.mv_size = docRecord.size();
    docValue.mv_data = (void*)docRecord.data();
    rc = mdb_put(txn, dbi_docs_, &docKey, &docValue, 0);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("Failed to put document record");
    }
    
    // Insert the URL mapping (url -> docid) into the urls DB.
    MDB_val putUrlKey, putUrlValue;
    putUrlKey.mv_size = url.size();
    putUrlKey.mv_data = (void*)url.data();
    std::string docIdStr = docidToStr(docId);
    putUrlValue.mv_size = docIdStr.size();
    putUrlValue.mv_data = (void*)docIdStr.data();
    rc = mdb_put(txn, dbi_urls_, &putUrlKey, &putUrlValue, 0);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("Failed to put URL mapping");
    }
    
    // Increment next_docid and update the meta DB.
    uint64_t nextDocId = docId + 1;
    metaData.mv_size = sizeof(nextDocId);
    metaData.mv_data = &nextDocId;
    rc = mdb_put(txn, dbi_meta_, &metaKey, &metaData, 0);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("Failed to update next_docid");
    }
    
    mdb_txn_commit(txn);
    return docId;
}

SearchRPI::docid DocDatabase::getDocId(const std::string& url) const {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("Failed to begin transaction in getDocId");
    
    MDB_val key, data;
    key.mv_size = url.size();
    key.mv_data = (void*)url.data();
    rc = mdb_get(txn, dbi_urls_, &key, &data);
    if (rc != MDB_SUCCESS) {
        mdb_txn_commit(txn);
        throw std::runtime_error("URL not found");
    }
    std::string docidStr((char*)data.mv_data, data.mv_size);
    uint64_t docId = std::stoull(docidStr);
    mdb_txn_commit(txn);
    return docId;
}

std::set<std::string> DocDatabase::getWords(SearchRPI::docid id) const {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("Failed to begin transaction in getWords");
    
    std::string keyStr = docidToStr(id);
    MDB_val key, data;
    key.mv_size = keyStr.size();
    key.mv_data = (void*)keyStr.data();
    rc = mdb_get(txn, dbi_docs_, &key, &data);
    if (rc != MDB_SUCCESS) {
        mdb_txn_commit(txn);
        throw std::runtime_error("Document not found");
    }
    
    std::string docData((char*)data.mv_data, data.mv_size);
    mdb_txn_commit(txn);
    
    std::string docUrl, docTitle;
    std::vector<std::string> words;
    deserializeDoc(docData, docUrl, docTitle, words);
    
    return std::set<std::string>(words.begin(), words.end());
}

bool DocDatabase::remove(SearchRPI::docid id) {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, 0, &txn);
    if (rc != MDB_SUCCESS)
        return false;
    
    std::string keyStr = docidToStr(id);
    MDB_val key, data;
    key.mv_size = keyStr.size();
    key.mv_data = (void*)keyStr.data();
    
    rc = mdb_get(txn, dbi_docs_, &key, &data);
    if (rc != MDB_SUCCESS) {
        mdb_txn_commit(txn);
        return false;
    }
    
    // Retrieve the document record to extract the URL.
    std::string docData((char*)data.mv_data, data.mv_size);
    std::string docUrl, docTitle;
    std::vector<std::string> words;
    deserializeDoc(docData, docUrl, docTitle, words);
    
    // Delete the document record from the docs DB.
    rc = mdb_del(txn, dbi_docs_, &key, nullptr);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        return false;
    }
    
    // Remove the URL mapping.
    MDB_val urlKey;
    urlKey.mv_size = docUrl.size();
    urlKey.mv_data = (void*)docUrl.data();
    rc = mdb_del(txn, dbi_urls_, &urlKey, nullptr);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        return false;
    }
    
    mdb_txn_commit(txn);
    return true;
}

int DocDatabase::getNumDocs() {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("mdb_txn_begin failed: " + std::string(mdb_strerror(rc)));

    MDB_stat stat;
    rc = mdb_stat(txn, dbi_docs_, &stat);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("mdb_stat failed: " + std::string(mdb_strerror(rc)));
    }

    mdb_txn_abort(txn); 

    return static_cast<int>(stat.ms_entries);
}


double DocDatabase::getAvgDocLen() {
    MDB_txn* txn;
    int rc = mdb_txn_begin(env_, nullptr, MDB_RDONLY, &txn);
    if (rc != MDB_SUCCESS)
        throw std::runtime_error("mdb_txn_begin failed: " + std::string(mdb_strerror(rc)));

    MDB_cursor* cursor = nullptr;
    rc = mdb_cursor_open(txn, dbi_docs_, &cursor);
    if (rc != MDB_SUCCESS) {
        mdb_txn_abort(txn);
        throw std::runtime_error("mdb_cursor_open failed: " + std::string(mdb_strerror(rc)));
    }

    MDB_stat stat;
    rc = mdb_stat(txn, dbi_docs_, &stat);
    if (rc != MDB_SUCCESS) {
        mdb_cursor_close(cursor);
        mdb_txn_abort(txn);
        throw std::runtime_error("mdb_stat failed: " + std::string(mdb_strerror(rc)));
    }

    if (stat.ms_entries == 0) {
        mdb_cursor_close(cursor);
        mdb_txn_abort(txn);
        return 0.0;
    }

    MDB_val key, data;
    long long docLens = 0;

    rc = mdb_cursor_get(cursor, &key, &data, MDB_FIRST);
    while (rc != MDB_NOTFOUND) {
        docLens += static_cast<long long>(data.mv_size);
        rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT);
    }

    mdb_cursor_close(cursor);
    mdb_txn_abort(txn);

    return static_cast<double>(docLens) / stat.ms_entries;
}

std::string DocDatabase::serializeDoc(const std::string& url, const std::string& title, const std::vector<std::string>& words) {
    std::ostringstream oss;
    oss << url << "\n" << title << "\n";
    for (size_t i = 0; i < words.size(); i++) {
        oss << words[i];
        if (i != words.size()-1)
            oss << ",";
    }
    return oss.str();
}

void DocDatabase::deserializeDoc(const std::string& data, std::string& url, std::string& title, std::vector<std::string>& words) const {
    std::istringstream iss(data);
    std::getline(iss, url);
    std::getline(iss, title);
    std::string wordsStr;
    std::getline(iss, wordsStr);
    words.clear();
    std::istringstream wss(wordsStr);
    std::string word;
    while (std::getline(wss, word, ',')) {
        if (!word.empty())
            words.push_back(word);
    }
}