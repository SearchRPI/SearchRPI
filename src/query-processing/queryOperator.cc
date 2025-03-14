#include "../../include/query-processing/queryOperator.h"

namespace queryTree {

// Define the operator mapping here
const std::unordered_map<std::string, QueryOperator> OPERATOR_MAP = {
    {"#text", QueryOperator::TEXT},
    {"#od", QueryOperator::OD},
    {"#root", QueryOperator::ROOT},
    {"#field", QueryOperator::FIELD},
    {"#dirichlet", QueryOperator::DIRICHLET},
    {"#jm", QueryOperator::JM},
    {"#linear", QueryOperator::LINEAR},
    {"#bil2", QueryOperator::BIL2},
    {"#bm25rf", QueryOperator::BM25RF},
    {"#bm25", QueryOperator::BM25},
    {"#bm25field", QueryOperator::BM25FIELD},
    {"#inl2", QueryOperator::INL2},
    {"#pl2", QueryOperator::PL2},
    {"#boost", QueryOperator::BOOST},
    {"#log", QueryOperator::LOG},
    {"#log-count", QueryOperator::LOG_COUNT},
    {"#combine", QueryOperator::COMBINE},
    {"#sdm", QueryOperator::SDM},
    {"#seqdep", QueryOperator::SEQDEP},
    {"#wsum", QueryOperator::WSUM},
    {"#require", QueryOperator::REQUIRE},
    {"#reject", QueryOperator::REJECT},
    {"#inside", QueryOperator::INSIDE},
    {"#greater", QueryOperator::GREATER},
    {"#less", QueryOperator::LESS},
    {"#between", QueryOperator::BETWEEN},
    {"#equals", QueryOperator::EQUALS},
    {"#passagefilter", QueryOperator::PASSAGEFILTER},
    {"#stopstructure", QueryOperator::STOPSTRUCTURE},
    {"#stopword", QueryOperator::STOPWORD},
    {"#counts", QueryOperator::COUNTS},
    {"#extents", QueryOperator::EXTENTS},
    {"#indicator", QueryOperator::INDICATOR},
    {"#lengths", QueryOperator::LENGTHS},
    {"#names", QueryOperator::NAMES},
    {"#prior", QueryOperator::PRIOR},
    {"#scores", QueryOperator::SCORES}
};

// Define the `toString` function in this `.cc` file
std::string toString(QueryOperator op) {
    switch (op) {
        case QueryOperator::TEXT: return "#text";
        case QueryOperator::OD: return "#od";
        case QueryOperator::ROOT: return "#root";
        case QueryOperator::FIELD: return "#field";
        case QueryOperator::DIRICHLET: return "#dirichlet";
        case QueryOperator::JM: return "#jm";
        case QueryOperator::LINEAR: return "#linear";
        case QueryOperator::BIL2: return "#bil2";
        case QueryOperator::BM25RF: return "#bm25rf";
        case QueryOperator::BM25: return "#bm25";
        case QueryOperator::BM25FIELD: return "#bm25field";
        case QueryOperator::INL2: return "#inl2";
        case QueryOperator::PL2: return "#pl2";
        case QueryOperator::BOOST: return "#boost";
        case QueryOperator::LOG: return "#log";
        case QueryOperator::LOG_COUNT: return "#log-count";
        case QueryOperator::COMBINE: return "#combine";
        case QueryOperator::SDM: return "#sdm";
        case QueryOperator::SEQDEP: return "#seqdep";
        case QueryOperator::WSUM: return "#wsum";
        case QueryOperator::REQUIRE: return "#require";
        case QueryOperator::REJECT: return "#reject";
        case QueryOperator::INSIDE: return "#inside";
        case QueryOperator::GREATER: return "#greater";
        case QueryOperator::LESS: return "#less";
        case QueryOperator::BETWEEN: return "#between";
        case QueryOperator::EQUALS: return "#equals";
        case QueryOperator::PASSAGEFILTER: return "#passagefilter";
        case QueryOperator::STOPSTRUCTURE: return "#stopstructure";
        case QueryOperator::STOPWORD: return "#stopword";
        case QueryOperator::COUNTS: return "#counts";
        case QueryOperator::EXTENTS: return "#extents";
        case QueryOperator::INDICATOR: return "#indicator";
        case QueryOperator::LENGTHS: return "#lengths";
        case QueryOperator::NAMES: return "#names";
        case QueryOperator::PRIOR: return "#prior";
        case QueryOperator::SCORES: return "#scores";
        case QueryOperator::UNKNOWN: return "UNKNOWN";
    }
    return "UNKNOWN";  // Fallback case
}

} // namespace queryTree
