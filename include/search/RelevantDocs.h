#pragma once

/**
 * @file  RelevantDocs.h
 * @brief Class containing set of documents considered relevant
*/

#include "types.h"

#include <unordered_set>

namespace SearchRPI {

// A set of documents determined relevant
class RelevantDocs {
public:
    
    RelevantDocs() = default;

    // Returns the number of relevant documents in the set.
    unsigned int size() const { return docs.size(); }
    
    // Checks if number of relevant documents is zero.
    bool empty() const { return size() == 0; }

    /** 
     *  @brief Mark a document as relevant.
     *  @note If @a doc_id is already marked as relevant, nothing happens.
     * 
     *  @param doc_id ID of relevant document.
     */
    void add_document(docid doc_id) { docs.insert(doc_id); }

    /** 
     *  @brief Unmark a document as relevant.
     *  @note If @a doc_id is not relevant, nothing happens.
     * 
     *  @param doc_id ID of irrelevant document.
     */
    void remove_document(docid doc_id) { docs.erase(doc_id); }

    /** 
     *  @param doc_id ID of document being checked.
     *  @returns Whether document is relevant.
     */
    bool contains(docid doc_id) const { return docs.count(doc_id) > 0; }

private:
    // NOTE: Depending on average size of these sets, normal set 
    //       may be more applicable than unordered_set.
    std::unordered_set<docid> docs;

};

}