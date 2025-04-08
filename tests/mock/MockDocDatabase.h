#pragma once

#include "IDocDatabase.h"
#include <gmock/gmock.h>

/**
 * @brief Mock class for IDocDatabase that uses Google Mock macros.
 */
class MockDocDB : public IDocDatabase {
public:
    MOCK_METHOD(bool, contains, (SearchRPI::docid id), (override));
    MOCK_METHOD(bool, contains, (const std::string& url), (override));
    MOCK_METHOD(bool, remove, (SearchRPI::docid id), (override));
    MOCK_METHOD(SearchRPI::docid, addDoc, (const std::string& url, const std::string& title, std::vector<std::string> words), (override));
    MOCK_METHOD(std::set<std::string>, getWords, (SearchRPI::docid id), (override));
    MOCK_METHOD(std::string, getDocText, (SearchRPI::docid id), (const, override));
    MOCK_METHOD(std::string, getDocTitle, (SearchRPI::docid id), (const, override));
    MOCK_METHOD(std::string, getDocUrl, (SearchRPI::docid id), (const, override));
};
