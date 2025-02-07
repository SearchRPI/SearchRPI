#pragma once

#include <math.h>

/**
 * @file  weight.h
 * @brief Weighting schemes
*/

namespace SearchRPI {

/**
 * @class Weight
 * @brief Base class for weighting schemes.
 */
class Weight {
public:

    Weight() = default;
    virtual ~Weight() = default;

    /**
     * @brief Virtual method to calculate the score of a document.
     * 
     * This method is intended to be overridden by derived classes to implement 
     * specific scoring logic.
     * 
     * TODO: Figure out way to handle different parameters depending on 
     *       weighting scheme.
     * 
     * @return The calculated score as a double.
     */
    virtual double get_score() const = 0;

};

class BM25Weight : Weight {
public:
    /**
     * @brief Constructor for BM25Weight.
     * 
     * @param k1 Scale factor for term frequency saturation. Default is 1.2.
     * @param b Length normalization impact. Default is 0.75.
     */
    BM25Weight(double k1 = 1.2, double b = 0.75) : k1(k1), b(b) {}

    /**
     * @brief Calculates the BM25 score for a document.
     * 
     * @param doc_len Length of the document.
     * @param term_freq Frequency of the term in the document.
     * @param avg_doc_len Average document length in the collection.
     * @param collection_size Total number of documents in the collection.
     * @return The BM25 score as a double.
     */
    double get_score(unsigned int doc_len,
                     unsigned int term_freq,
                     double avg_doc_len,
                     unsigned int collection_size,
                     unsigned int doc_freq) const;

protected:
    // Scale Factor
    double k1;

    // Length normalization impact
    double b;
};

// TODO: BM25Plus? BM25f?

// TF-IDF Implementation
class TFIDFWeight : Weight {
public:
    /**
     * @brief Constructor for TFIDFWeight.
     * 
     * @param log_base Base of the log used in IDF calculation. Default is 10.
     */
    TFIDFWeight(unsigned int log_base = 10) : log_base(log_base) {}

    /**
     * @brief Calculates the TF-IDF score for a document.
     * 
     * @param term_freq Frequency of the term in the document.
     * @param avg_doc_len Average document length in the collection.
     * @param collection_size Total number of documents in the collection.
     * @return The TF-IDF score as a double.
     */
    double get_score(unsigned int term_freq,
                     double avg_doc_len,
                     unsigned int collection_size,
                     unsigned int doc_freq) const;

protected:
    // Base of the logarithm used in IDF calculation.
    unsigned int log_base;
};

}