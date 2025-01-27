#include <lmdb.h>
#include <iostream>
#include <string>
#include <cstring>

// Custom comparison function for sorting values
int custom_compare(const MDB_val *a, const MDB_val *b) {
    // Sort values by string length first, then lexicographically
    size_t len_a = a->mv_size;
    size_t len_b = b->mv_size;
    if (len_a != len_b) {
        return (len_a < len_b) ? -1 : 1;
    }
    return std::memcmp(a->mv_data, b->mv_data, len_a);
}

int main() {
    const char *db_path = "../testdb";
    MDB_env *env = nullptr;
    MDB_dbi dbi;
    MDB_txn *txn;

    // Initialize LMDB environment
    if (mdb_env_create(&env) != 0) {
        std::cerr << "Failed to create LMDB environment." << std::endl;
        return 1;
    }
    if (mdb_env_set_mapsize(env, 10 * 1024 * 1024) != 0) {
        std::cerr << "Failed to set map size." << std::endl;
        return 1;
    }
    if (mdb_env_open(env, db_path, MDB_CREATE, 0664) != 0) {
        std::cerr << "Failed to open LMDB environment." << std::endl;
        return 1;
    }

    // Begin a write transaction
    if (mdb_txn_begin(env, nullptr, 0, &txn) != 0) {
        std::cerr << "Failed to begin transaction." << std::endl;
        return 1;
    }

    // Open the database with MDB_DUPSORT to allow duplicate values
    if (mdb_dbi_open(txn, nullptr, MDB_CREATE | MDB_DUPSORT, &dbi) != 0) {
        std::cerr << "Failed to open database." << std::endl;
        mdb_txn_abort(txn);
        return 1;
    }

    // Set the custom comparison function for duplicate values
    if (mdb_set_dupsort(txn, dbi, custom_compare) != 0) {
        std::cerr << "Failed to set custom comparison function." << std::endl;
        mdb_txn_abort(txn);
        return 1;
    }

    // Insert multiple values for the same key
    const char *key_str = "example_key";
    std::string values[] = {"value1", "value3", "value2"};

    MDB_val key, value;
    key.mv_size = std::strlen(key_str);
    key.mv_data = (void *)key_str;

    for (const auto &val : values) {
        value.mv_size = val.size();
        value.mv_data = (void *)val.c_str();
        if (mdb_put(txn, dbi, &key, &value, 0) != 0) {
            std::cerr << "Failed to insert value: " << val << std::endl;
            mdb_txn_abort(txn);
            return 1;
        }
    }

    // Commit the transaction
    if (mdb_txn_commit(txn) != 0) {
        std::cerr << "Failed to commit transaction." << std::endl;
        return 1;
    }

    // Begin a read-only transaction to verify the values
    if (mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn) != 0) {
        std::cerr << "Failed to begin read transaction." << std::endl;
        return 1;
    }

    // Retrieve and print all values for the key
    MDB_cursor *cursor;
    if (mdb_cursor_open(txn, dbi, &cursor) != 0) {
        std::cerr << "Failed to open cursor." << std::endl;
        return 1;
    }

    std::cout << "Values for key \"" << key_str << "\":" << std::endl;
    MDB_val dup_value;
    while (mdb_cursor_get(cursor, &key, &dup_value, MDB_GET_MULTIPLE) == 0 ||
           mdb_cursor_get(cursor, &key, &dup_value, MDB_NEXT_DUP) == 0) {
        std::cout << "- " << std::string((char *)dup_value.mv_data, dup_value.mv_size) << std::endl;
    }

    // Cleanup
    mdb_cursor_close(cursor);
    mdb_txn_abort(txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    return 0;
}
