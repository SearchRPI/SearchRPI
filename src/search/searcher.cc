#include "search/searcher.h"
#include "index/IDatabase.h"

#include <unordered_map>

namespace Ranking {

MatchingDocs Searcher::Search(const Query& query, unsigned int max_items) {

    // NOTE: CURRENT IMPLEMENTATION IS TEMPORARY

    std::vector<std::string> terms = query.terms();
    std::unordered_map<int, double> mdocs; // mdocs[docid] = score;
    
    int avg_doc_len = 1;
    int collection_size = 2;

    for (const std::string& term : terms) {
        std::vector<Data> docs = db->get(term, 100000);

        for (const Data& doc : docs) {
            int freq = doc.priority;
            int docid = doc.docId;

            int doc_len = 1;
            int doc_freq = 1;

            double score = weight_scheme->get_score(doc_len, freq, avg_doc_len, collection_size, doc_freq);
            mdocs[docid] += score;
        }
    }

    std::vector<SearchResult> results;
    for (const auto& [doc_id, score] : mdocs) {
        results.emplace_back(score, doc_id);
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
         return a.get_weight() > b.get_weight();
    });

    if (results.size() > max_items) {
        // This is not efficient at all but works for now
        results.erase(results.begin() + max_items, results.end());
    }

    MatchingDocs matching;
    for (const auto& result : results) {
        matching.add_result(result);
    }

    return matching;
}

}
