#ifndef DATABASEWRAPPER_H
#define DATABASEWRAPPER_H

#include <lmdb.h>
#include <string>

// Struct to store in LMDB
struct Data {
    int priority;         // Priority value (used for ranking)
    std::string value;    // Actual string data
};

class DatabaseWrapper {
public:
    // Get the singleton instance
    static DatabaseWrapper& getInstance();

    // Delete copy constructor and assignment operator
    DatabaseWrapper(const DatabaseWrapper&) = delete;
    DatabaseWrapper& operator=(const DatabaseWrapper&) = delete;

    // Add data to the database
    void add(const std::string& key, const Data& data);

    // Remove data for a specific key
    void remove(const std::string& key);

    // Retrieve the first value for a given key
    Data get(const std::string& key);

private:
    MDB_env* env = nullptr;
    MDB_dbi dbi;
    MDB_txn* write_txn = nullptr;
    MDB_txn* read_txn = nullptr;

    // Private constructor for singleton
    DatabaseWrapper();

    // Destructor to clean up LMDB resources
    ~DatabaseWrapper();

    // Serialize the Data struct
    void serialize_data(const Data& data, MDB_val& value);

    // Deserialize the Data struct
    void deserialize_data(const MDB_val& value, Data& data);

    // Custom comparison function for ranking by priority
    static int custom_compare(const MDB_val* a, const MDB_val* b);
};

#endif // LMDBWRAPPER_H
