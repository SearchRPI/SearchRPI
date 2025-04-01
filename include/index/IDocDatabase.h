#pragma once

/**
 * @file  IDocDatabase.h
 * @brief Interface for database containing document information
*/

#include "types.h"

#include <string>
#include <vector>
#include <set>

/**
 * @brief Interface for Document Databases
 */
class IDocDatabase {
public:
    IDocDatabase() = default;
    virtual ~IDocDatabase() = default;

    // Disable Copy Constructor/Assignment Operator
    IDocDatabase(const IDocDatabase&) = delete;
    IDocDatabase& operator=(IDocDatabase const&) = delete;

    /**
     * @param id Document ID
     * @returns Whether corups contains a document with that ID
     */
    virtual bool contains(SearchRPI::docid id) const = 0;

    /**
     * @param url URL of Site (e.g. sis.rpi.edu) (strip https://)
     * @returns Whether any document is located at that url
     */
    virtual bool contains(const std::string& url) const = 0;

    /**
     * @param url Page URL
     * @param title Page Title (Shown in search results)
     * @param words Text of the page // TODO: Update format to include punctuation & support field weighting
     * @returns ID Generated for Added Document
     */
    virtual SearchRPI::docid addDoc(const std::string& url, const std::string& title, const std::string& rawText) = 0;

    /**
     * @param url Page URL
     * @returns ID associated with URL
     */
    virtual SearchRPI::docid getDocId(const std::string& url) const = 0;

    /**
     * @param id Document ID
     * @returns A set of all words in the document
     */
    virtual std::set<std::string> getWords(SearchRPI::docid id) const = 0;

    /**
     * @param id Document ID
     * @returns Whether document existed and was removed (false if doc didn't exist)
     */
    virtual bool remove(SearchRPI::docid id) = 0;

    /**
     * @param id Document ID
     * @returns The complete original text of the document.
     */
    virtual std::string getDocText(SearchRPI::docid id) const = 0;

    /**
     * @param id Document ID
     * @returns The title of the document
     */
    virtual std::string getDocTitle(SearchRPI::docid id) const = 0;

    /**
     * @param id Document ID
     * @returns The URL of the document
     */
    virtual std::string getDocUrl(SearchRPI::docid id) const = 0;

};
