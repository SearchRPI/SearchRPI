#include "DatabaseWrapper.h"
#include <cstring>
#include <stdexcept>

// Get the singleton instance
DatabaseWrapper& DatabaseWrapper::getInstance() {
    static DatabaseWrapper instance;
    return instance;
}

// Private constructor for singleton
DatabaseWrapper::DatabaseWrapper() {
    const char* db_path = "../testdb";

    // Initialize LMDB environment
    if (mdb_env_create(&env) != 0) {
        throw std::runtime_error("Failed to create LMDB environment.");
    }
    if (mdb_env_set_mapsize(env, 10 * 1024 * 1024) != 0) {
        throw std::runtime_error("Failed to set map size.");
    }
    if (mdb_env_open(env, db_path, MDB_CREATE, 0664) != 0) {
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
DatabaseWrapper::~DatabaseWrapper() {
    if (read_txn) mdb_txn_abort(read_txn);
    if (write_txn) mdb_txn_commit(write_txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);
}

// Serialize the Data struct
void DatabaseWrapper::serialize_data(const Data& data, MDB_val& value) {
    size_t value_size = data.value.size();
    size_t total_size = sizeof(int) + value_size;
    void* data_blob = malloc(total_size);

    std::memcpy(data_blob, &data.priority, sizeof(int));
    std::memcpy((char*)data_blob + sizeof(int), data.value.c_str(), value_size);

    value.mv_size = total_size;
    value.mv_data = data_blob;
}

// Deserialize the Data struct
void DatabaseWrapper::deserialize_data(const MDB_val& value, Data& data) {
    std::memcpy(&data.priority, value.mv_data, sizeof(int));
    size_t value_size = value.mv_size - sizeof(int);
    data.value = std::string((char*)value.mv_data + sizeof(int), value_size);
}

// Custom comparison function for ranking by priority
int DatabaseWrapper::custom_compare(const MDB_val* a, const MDB_val* b) {
    int priority_a, priority_b;
    std::memcpy(&priority_a, a->mv_data, sizeof(int));
    std::memcpy(&priority_b, b->mv_data, sizeof(int));
    return priority_a - priority_b;
}

// Add data to the database
void DatabaseWrapper::add(const std::string& key, const Data& data) {
    MDB_val mdb_key, mdb_value;
    mdb_key.mv_size = key.size();
    mdb_key.mv_data = (void*)key.c_str();

    serialize_data(data, mdb_value);

    int rc = mdb_put(write_txn, dbi, &mdb_key, &mdb_value, 0);
    free(mdb_value.mv_data); // Free allocated memory

    if (rc != 0) {
        throw std::runtime_error("Failed to add data: " + std::string(mdb_strerror(rc)));
    }

    // Ensure transaction commits
    mdb_txn_commit(write_txn);
    mdb_txn_begin(env, nullptr, 0, &write_txn);
}

// Remove data for a specific key
void DatabaseWrapper::remove(const std::string& key) {
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

// Retrieve the first value for a given key
Data DatabaseWrapper::get(const std::string& key) {
    MDB_val mdb_key, mdb_value;
    mdb_key.mv_size = key.size();
    mdb_key.mv_data = (void*)key.c_str();

    if (mdb_get(read_txn, dbi, &mdb_key, &mdb_value) == 0) {
        Data data;
        deserialize_data(mdb_value, data);
        return data;
    } else {
        throw std::runtime_error("Key not found: " + key);
    }
}
