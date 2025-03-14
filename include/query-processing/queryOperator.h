#pragma once

#include <unordered_map>
#include <string>

/**
  ******************************************************************************
  * @file           : queryOperator.h
  * @brief          : An enum holding all the Galago query operators.
  ******************************************************************************
*/

#pragma once

#include <unordered_map>
#include <string>

namespace queryTree {

// Enum definitions remain unchanged
enum class QueryOperator {
    TEXT, ROOT, FIELD, DIRICHLET, JM, LINEAR, BIL2, BM25RF, BM25, BM25FIELD,
    INL2, PL2, BOOST, LOG, LOG_COUNT, MINCOUNT, COUNT_SUM, SYNONYM, ORDERED,
    OD, BIGRAM, QUOTE, ODN, UNORDERED, UW, UWN, UBIGRAM, INTERSECT, NULL_OP,
    COMBINE, SDM, SEQDEP, WSDM, FIELDEDSDM, FDM, FULLDEP, PDFR, PRMS, PRMS2,
    WSUM, MLM, PL2F, WPHRANK, THRESHOLD, SCALE, RM, FIELDRM, BM25FCOMB,
    LOGPROBNOT, ALL, BAND, ANY, BOR, BNOT, BOOL_TO_COUNT, BOOL, REQUIRE,
    REJECT, INSIDE, GREATER, LESS, BETWEEN, EQUALS, PASSAGEFILTER, PL2SCORER,
    PASSAGELENGTHS, STOPSTRUCTURE, STOPWORD, COUNTS, EXTENTS, INDICATOR,
    LENGTHS, NAMES, PRIOR, SCORES, UNKNOWN
};

// Mapping string operators to enum values remains unchanged
extern const std::unordered_map<std::string, QueryOperator> OPERATOR_MAP;

// Function prototype only
std::string toString(QueryOperator op);

} // namespace queryTree
