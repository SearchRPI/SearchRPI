#pragma once

#include "types.h"

#include <string>
#include <vector>

/*
 *  Document Database
 *  Can find document using either complete URL or Doc ID.
 *  Database is responsible for generating new ID if added
 *  Document does not currently exist.
 */

/**
 * @brief Interface for Database operations
 */
class IDocDatabase {
public:
    IDocDatabase() = default;
    virtual ~IDocDatabase() = default;

    /**
     * @param id Document ID
     * @returns Whether corups contains a document with that ID
     */
    virtual bool contains(SearchRPI::docid id) const = 0;

    /**
     * @param url URL of Site (e.g. sis.rpi.edu) (strip https://)
     * @returns Whether any document is located at that url
     */
    virtual bool contains(std::string url) const = 0;

    virtual SearchRPI::docid updateDoc(std::string url, std::string title, std::string text) const = 0;

};
