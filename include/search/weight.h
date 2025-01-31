#pragma once

/**
 * @file  weight.h
 * @brief Weighting schemes
*/

namespace SearchRPI {

// TODO: Ranking Algorithm Here (BM25)
//       Use class so swap to BM25f (Incorporating Field Weights) will be easier

class Weight {
public:


    virtual double Score();

protected:
    unsigned int collection_size;
    double avg_doc_len;

    unsigned int num_relevant_docs;

    unsigned int term_freq;
    unsigned int collection_freq;

};

class BM25Weight : Weight {
public:

protected:
    // Scale Factor (default = 1.2)
    double k1;

    // Length normalization impact (default = 0.75)
    double b;

};

class TFIDFWeight : Weight {

};

}