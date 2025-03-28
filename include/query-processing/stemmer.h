#pragma once

/**
  ******************************************************************************
  * @file           : stemmer.h
  * @brief          : Implements the porter stemming algorithm.
  * @date           : 2/4/2025
  ******************************************************************************
*/

#include <string>

namespace stemmer {

/**
 * @brief Reduces a word to its stem using the Porter stemming algorithm.
 *
 * This function processes the input word and returns its stemmed form.
 *
 * @param word The word to be stemmed.
 * @return std::string The stemmed version of the input word.
 */
std::string stem(const std::string &word);

} // namespace stemmer
