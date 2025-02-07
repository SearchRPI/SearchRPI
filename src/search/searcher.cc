#include "../../include/search/searcher.h"


MatchingDocs Searcher::Search(const Query& query, unsigned int max_items) {

    // get query terms
    std::vector<std::string> terms;
    for (const std::string& term : terms) {
        std::vector<Data> = db->get(term, 100000);
        int freq = Data.priority;
        int docid = Data.docid;
        double score = weight_scheme.get_score()
    }

}