#include "Database.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

// Constructor with database path
Database::Database(const std::string& db_path) {
    // Initialize LMDB environment
    if (mdb_env_create(&env) != 0) {
        throw std::runtime_error("Failed to create LMDB environment.");
    }
    if (mdb_env_set_mapsize(env, 10 * 1024 * 1024) != 0) {
        throw std::runtime_error("Failed to set map size.");
    }
    if (mdb_env_open(env, db_path.c_str(), MDB_CREATE, 0664) != 0) {
        throw std::runtime_error("Failed to open LMDB environment.");
    }

    // Start write and read transactions
    if (mdb_txn_begin(env, nullptr, 0, &write_txn) != 0) {
        throw std::runtime_error("Failed to begin write transaction.");
    }
    if (mdb_txn_begin(env, nullptr, MDB_RDONLY, &read_txn) != 0) {
        throw std::runtime_error("Failed to begin read transaction.");
    }

    // Open database with MDB_DUPSORT for duplicate support
    if (mdb_dbi_open(write_txn, nullptr, MDB_CREATE | MDB_DUPSORT, &dbi) != 0) {
        throw std::runtime_error("Failed to open database.");
    }

    // Set custom comparison function
    mdb_set_dupsort(write_txn, dbi, custom_compare);
}

// Destructor to clean up LMDB resources
Database::~Database() {
    if (read_txn) mdb_txn_abort(read_txn);
    if (write_txn) mdb_txn_commit(write_txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);
}

// Serialize the Data struct
void Database::serialize_data(const Data& data, MDB_val& value) {
    size_t total_size = sizeof(int) * 2; // Two integers
    void* data_blob = malloc(total_size);
    if (!data_blob) {
        throw std::bad_alloc();
    }

    std::memcpy(data_blob, &data.priority, sizeof(int));
    std::memcpy((char*)data_blob + sizeof(int), &data.docId, sizeof(int));

    value.mv_size = total_size;
    value.mv_data = data_blob;
}

void Database::deserialize_data(const MDB_val& value, Data& data) {
    if (value.mv_size != sizeof(int) * 2) {
        throw std::runtime_error("Invalid data size during deserialization");
    }
    std::memcpy(&data.priority, value.mv_data, sizeof(int));
    std::memcpy(&data.docId, (char*)value.mv_data + sizeof(int), sizeof(int));
}

// Custom comparison function for ranking by priority
int Database::custom_compare(const MDB_val* a, const MDB_val* b) {
    int priority_a, priority_b;
    std::memcpy(&priority_a, a->mv_data, sizeof(int));
    std::memcpy(&priority_b, b->mv_data, sizeof(int));
    return priority_a - priority_b;
}

// Add data to the database
void Database::add(const std::string& key, const Data& data) {
    MDB_val mdb_key, mdb_value;
    mdb_key.mv_size = key.size();
    mdb_key.mv_data = (void*)key.c_str();

    serialize_data(data, mdb_value);

    int rc = mdb_put(write_txn, dbi, &mdb_key, &mdb_value, 0);
    free(mdb_value.mv_data);

    if (rc != 0) {
        throw std::runtime_error("Failed to add data: " + std::string(mdb_strerror(rc)));
    }

    // Ensure transaction commits
    mdb_txn_commit(write_txn);
    mdb_txn_begin(env, nullptr, 0, &write_txn);
}

// Remove data for a specific key
void Database::remove(const std::string& key) {
    MDB_val mdb_key;
    mdb_key.mv_size = key.size();
    mdb_key.mv_data = (void*)key.c_str();

    // Check if key exists before deleting
    MDB_val dummy_value;
    int exists = mdb_get(write_txn, dbi, &mdb_key, &dummy_value);

    if (exists == MDB_NOTFOUND) {
        throw std::runtime_error("Key not found: " + key);
    }

    int rc = mdb_del(write_txn, dbi, &mdb_key, nullptr);
    if (rc != 0 && rc != MDB_NOTFOUND) {
        throw std::runtime_error("Failed to remove key: " + std::string(mdb_strerror(rc)));
    }

    // Ensure transaction commits
    mdb_txn_commit(write_txn);
    mdb_txn_begin(env, nullptr, 0, &write_txn);
}

std::vector<Data> Database::get(const std::string& key) {
    MDB_val mdb_key, mdb_value;
    mdb_key.mv_size = key.size();
    mdb_key.mv_data = (void*)key.c_str();

    MDB_cursor* cursor;
    if (mdb_cursor_open(read_txn, dbi, &cursor) != 0) {
        throw std::runtime_error("Failed to open LMDB cursor");
    }

    std::vector<Data> results;
    if (mdb_cursor_get(cursor, &mdb_key, &mdb_value, MDB_SET) == 0) {
        do {
            Data data;
            deserialize_data(mdb_value, data);
            results.push_back(data);
        } while (mdb_cursor_get(cursor, &mdb_key, &mdb_value, MDB_NEXT_DUP) == 0);
    }

    mdb_cursor_close(cursor);

    if (results.empty()) {
        throw std::runtime_error("Key not found: " + key);
    }

    return results;
}

std::vector<Data> Database::get(const std::string& key, size_t n) {
    MDB_val mdb_key, mdb_value;
    mdb_key.mv_size = key.size();
    mdb_key.mv_data = (void*)key.c_str();

    MDB_cursor* cursor;
    if (mdb_cursor_open(read_txn, dbi, &cursor) != 0) {
        throw std::runtime_error("Failed to open LMDB cursor");
    }

    std::vector<Data> results;
    size_t count = 0;

    if (mdb_cursor_get(cursor, &mdb_key, &mdb_value, MDB_SET) == 0) {
        do {
            Data data;
            deserialize_data(mdb_value, data);
            results.push_back(data);
            count++;
        } while (count < n && mdb_cursor_get(cursor, &mdb_key, &mdb_value, MDB_NEXT_DUP) == 0);
    }

    mdb_cursor_close(cursor);

    if (results.empty()) {
        throw std::runtime_error("Key not found: " + key);
    }

    return results;
}
