#include "../../include/search/weight.h"

double SearchRPI::BM25Weight::get_score(unsigned int doc_len, unsigned int term_freq, double avg_doc_len, unsigned int collection_size, unsigned int doc_freq) const {
    if (doc_freq == 0){
        return 0.0;
    }

    double idf = std::log((collection_size - doc_freq + 0.5) / (doc_freq + 0.5) + 1);
    double tf = (term_freq * (k1 + 1)) / (term_freq + k1 * (1 - b + b * (doc_len / avg_doc_len)));

    return idf * tf;
}

double SearchRPI::TFIDFWeight::get_score(unsigned int term_freq, double avg_doc_len, unsigned int collection_size, unsigned int doc_freq) const {
    if (avg_doc_len == 0){
        return 0.0;
    }

    double idf = std::log(collection_size / (doc_freq + 1)) / std::log(log_base);

    return term_freq * idf;
}
