#include "search/searcher.h"
#include "index/IDatabase.h"

namespace SearchRPI {

MatchingDocs Searcher::Search(const Query& query, unsigned int max_items) {

    // get query terms
    std::vector<std::string> terms;
    for (const std::string& term : terms) {
        std::vector<Data> docs = db->get(term, 100000);
        int freq = Data.priority;
        int docid = Data.docid;
        double score = weight_scheme.get_score()
    }

}

// MatchingDocs Searcher::Search(const Query& query, unsigned int max_items) 
// {
//     // Example: assume the query has 1 term "foo"
//     // or you iterate over query terms, etc.
//     std::string key = query.someQueryKey;
//     std::vector<Data> rawDocs = db->get(key);
    
//     // "Ranking" or weighting using weight_scheme_ ...
//     // transform into MatchingDocs ... 
//     // for example:
//     MatchingDocs results;
//     for (const auto& d : rawDocs) {
//         results.push_back({d.docId, /*some score based on weight_scheme_*/ 1.0});
//     }

//     // Sort results by some criterion or do something else
//     // Then trim to max_items
//     if (results.size() > max_items) {
//         results.resize(max_items);
//     }

//     return results;
// }

}
