#include "search/searcher.h"
#include "index/IDatabase.h"

#include <unordered_map>

namespace SearchRPI {

MatchingDocs Searcher::Search(const Query& query, unsigned int max_items) {

    // NOTE: CURRENT IMPLEMENTATION IS TEMPORARY

    std::vector<std::string> terms = query.terms();
    std::unordered_map<int, double> mdocs; // mdocs[docid] = score;
    
    int avg_doc_len = 0;
    int collection_size = 0;

    for (const std::string& term : terms) {
        std::vector<Data> docs = db->get(term, 100000);

        for (const Data& doc : docs) {
            int freq = doc.priority;
            int docid = doc.docId;

            int doc_len = 0;
            int doc_freq = 0;

            double score = weight_scheme.get_score(doc_len, freq, avg_doc_len, collection_size, doc_freq);
            mdocs[docid] += score;
        }
    }

    // Convert map to a MatchingDocs object
    MatchingDocs results;
    for (const auto& [doc_id, score] : mdocs) {
        results.add_result(SearchResult(score, doc_id));
    }

    // Sort descending
    results.sort_by_score_desc();

    // Return final results
    return results;
}

}
