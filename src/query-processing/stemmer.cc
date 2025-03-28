#include "../../include/query-processing/stemmer.h"
#include <string>
#include <cctype>


bool is_vowel(char ch) {
    ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u');
}

bool isConsonant(const std::string &word, int i) {
    char ch = word[i];
    switch (ch) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            return false;
        case 'y':
            return (i == 0) ? true : !isConsonant(word, i - 1);
        default:
            return true;
    }
}

int measure(const std::string &word) {
    int count = 0;
    int i = 0;
    int n = word.size();
    // Skip initial consonants.
    while (i < n && isConsonant(word, i))
        i++;
    while (i < n) {
        // Skip a sequence of vowels.
        while (i < n && !isConsonant(word, i))
            i++;
        count++;
        // Skip a sequence of consonants.
        while (i < n && isConsonant(word, i))
            i++;
    }
    return count;
}

bool containsVowel(const std::string &word) {
    for (size_t i = 0; i < word.size(); ++i) {
        if (!isConsonant(word, i))
            return true;
    }
    return false;
}

bool endsWith(const std::string &word, const std::string &suffix) {
    if (word.size() < suffix.size())
        return false;
    return word.compare(
        word.size() - suffix.size(), suffix.size(), suffix
    ) == 0;
}

bool cvc(const std::string &word) {
    int n = word.size();
    if (n < 3)
        return false;
    if (!isConsonant(word, n - 3) || !(!isConsonant(word, n - 2))
        || !isConsonant(word, n - 1))
        return false;
    char ch = word[n - 1];
    if (ch == 'w' || ch == 'x' || ch == 'y')
        return false;
    return true;
}

void step1a(std::string &word) {
    if (endsWith(word, "sses")) {
        word.replace(word.size() - 4, 4, "ss");
    } else if (endsWith(word, "ies")) {
        word.replace(word.size() - 3, 3, "i");
    } else if (endsWith(word, "ss")) {
        // do nothing
    } else if (endsWith(word, "s")) {
        word.erase(word.size() - 1);
    }
}

void step1b(std::string &word) {
    bool flag = false;
    if (endsWith(word, "eed")) {
        std::string stem = word.substr(0, word.size() - 3);
        if (measure(stem) > 0) {
            word.replace(word.size() - 3, 3, "ee");
        }
    } else if ((endsWith(word, "ed") && 
                containsVowel(word.substr(0, word.size() - 2))) ||
               (endsWith(word, "ing") && 
                containsVowel(word.substr(0, word.size() - 3)))) {
        if (endsWith(word, "ed"))
            word.erase(word.size() - 2);
        else
            word.erase(word.size() - 3);
        flag = true;
    }
    if (flag) {
        if (endsWith(word, "at") || endsWith(word, "bl")
            || endsWith(word, "iz")) {
            word.push_back('e');
        } else if (word.size() >= 2 &&
                   word[word.size() - 1] == word[word.size() - 2] &&
                   word[word.size() - 1] != 'l' &&
                   word[word.size() - 1] != 's' &&
                   word[word.size() - 1] != 'z') {
            word.erase(word.size() - 1);
        } else if (measure(word) == 1 && cvc(word)) {
            word.push_back('e');
        }
    }
}

void step1c(std::string &word) {
    if (endsWith(word, "y") && word.size() > 1 && isConsonant(word, word.size() - 2)) {
        word[word.size() - 1] = 'i';
    }
}

void step2(std::string &word) {
    struct SuffixRule {
        const char *suffix;
        const char *replacement;
    };
    SuffixRule rules[] = {
        {"ational", "ate"}, {"tional", "tion"}, {"enci", "ence"},
        {"anci", "ance"},   {"izer", "ize"},   {"abli", "able"},
        {"alli", "al"},     {"entli", "ent"},  {"eli", "e"},
        {"ousli", "ous"},   {"ization", "ize"},{"ation", "ate"},
        {"ator", "ate"},    {"alism", "al"},   {"iveness", "ive"},
        {"fulness", "ful"}, {"ousness", "ous"},{"aliti", "al"},
        {"iviti", "ive"},   {"biliti", "ble"}
    };
    for (auto &rule : rules) {
        std::string suffix(rule.suffix);
        if (endsWith(word, suffix)) {
            std::string stem = word.substr(0, word.size() - suffix.size());
            if (measure(stem) > 0) {
                word.replace(
                    word.size() - suffix.size(), suffix.size(), rule.replacement
                );
            }
            break;
        }
    }
}

void step3(std::string &word) {
    struct SuffixRule {
        const char *suffix;
        const char *replacement;
    };
    SuffixRule rules[] = {
        {"icate", "ic"}, {"ative", ""}, {"alize", "al"},
        {"iciti", "ic"}, {"ical", "ic"}, {"ful", ""},
        {"ness", ""}
    };
    for (auto &rule : rules) {
        std::string suffix(rule.suffix);
        if (endsWith(word, suffix)) {
            std::string stem = word.substr(0, word.size() - suffix.size());
            if (measure(stem) > 0) {
                word.replace(
                    word.size() - suffix.size(), suffix.size(), rule.replacement
                );
            }
            break;
        }
    }
}

void step4(std::string &word) {
    const char* suffixes[] = {
        "al", "ance", "ence", "er", "ic", "able", "ible",
        "ant", "ement", "ment", "ent", "ion", "ou", "ism",
        "ate", "iti", "ous", "ive", "ize"
    };
    for (auto s : suffixes) {
        std::string suffix(s);
        if (endsWith(word, suffix)) {
            std::string stem = word.substr(0, word.size() - suffix.size());
            if (measure(stem) > 1) {
                if (std::string(s) == "ion") {
                    if (!stem.empty() && (stem.back() == 's' || stem.back() == 't'))
                        word.erase(word.size() - suffix.size());
                } else {
                    word.erase(word.size() - suffix.size());
                }
                break;
            }
        }
    }
}

void step5(std::string &word) {
    // Step 5a: Remove trailing 'e'
    if (endsWith(word, "e")) {
        std::string stem = word.substr(0, word.size() - 1);
        int m = measure(stem);
        if (m > 1 || (m == 1 && !cvc(stem)))
            word.erase(word.size() - 1);
    }
    // Step 5b: Remove double 'l' if measure > 1.
    if (measure(word) > 1 && word.size() >= 2 &&
        word[word.size() - 1] == 'l' && word[word.size() - 2] == 'l') {
        word.erase(word.size() - 1);
    }
}

namespace stemmer {

std::string stem(const std::string &input) {
    std::string word = input;
    if (word.size() <= 2)
        return word; // Too short to stem reliably.
    step1a(word);
    step1b(word);
    step1c(word);
    step2(word);
    step3(word);
    step4(word);
    step5(word);
    return word;
}

} // namespace porter_stemmer
