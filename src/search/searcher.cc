#include "search/searcher.h"
#include "index/IDatabase.h"

#include <unordered_map>

namespace Ranking {

MatchingDocs Searcher::Search(const queryTree::QueryTree& queryTree, unsigned int max_items) {

    int avg_doc_len = 20;
    int collection_size = 2;
    std::unordered_map<int, double> mdocs; // mdocs[docid] = score;

    queryTree.forEachNodeWithDepth([&](const queryTree::QueryNode& node, int depth) {
        if (node.getOperation() != queryTree::QueryOperator::TEXT)
            return;

        std::string term = node.getValue();
        std::vector<Data> docs = db->get(term, 100000);

        for (const Data& doc : docs) {
            int freq = doc.priority;
            int docid = doc.docId;

            int doc_len = 10;
            int doc_freq = 10;

            double score = weight_scheme->get_score(
                doc_len,
                freq,
                avg_doc_len,
                collection_size,
                doc_freq
            );
            mdocs[docid] += score;
        }
    });

    std::vector<SearchResult> results;
    for (const auto& [doc_id, score] : mdocs) {
        results.emplace_back(score, doc_id);
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
         return a.get_weight() > b.get_weight();
    });

    MatchingDocs matching;
    for (int i = 0; i < std::min((unsigned int) results.size(), max_items); i++) {
        matching.add_result(results[i]);
    }

    return matching;
}

}