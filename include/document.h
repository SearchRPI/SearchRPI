#pragma once

/**
 * @file  document.h
 * @brief Class for handling documents
*/

#include "types.h"

#include<string>
#include<unordered_map>

namespace SearchRPI {

class Document {
public:
    Document() = delete;

protected:
    SearchRPI::docid id;
    std::unordered_map<std::string, int> term_freqs;
    double url_rating;

    // TODO: age?
};

}