#include <gtest/gtest.h>
#include "../include/query-processing/stemmer.h"

// Test fixture for the stemmer
class StemmerTest : public ::testing::Test {
protected:
    std::string stem(const std::string& word) {
        return stemmer::stem(word);
    }
};

// Test basic stemming of common words
TEST_F(StemmerTest, BasicStemming) {
    EXPECT_EQ(stem("running"), "run");
    EXPECT_EQ(stem("happiness"), "happi");
    EXPECT_EQ(stem("flying"), "fli");
}

// Test stemming of words ending in "y"
TEST_F(StemmerTest, WordsEndingInY) {
    EXPECT_EQ(stem("happy"), "happi");  // 'y' → 'i'
    EXPECT_EQ(stem("cry"), "cri");      // 'y' → 'i'
    EXPECT_EQ(stem("toy"), "toy");      // Should not change (preceded by vowel)
    EXPECT_EQ(stem("enjoy"), "enjoy");  // Should not change (preceded by vowel)
}

// Test stemming of plural forms
TEST_F(StemmerTest, Plurals) {
    EXPECT_EQ(stem("caresses"), "caress");
    EXPECT_EQ(stem("ponies"), "poni");
    EXPECT_EQ(stem("ties"), "ti");
}

// Test stemming of common suffixes
TEST_F(StemmerTest, CommonSuffixes) {
    EXPECT_EQ(stem("national"), "nation");
    EXPECT_EQ(stem("relational"), "relat");
    EXPECT_EQ(stem("conditional"), "condit");
}

// Test words that should not change
TEST_F(StemmerTest, WordsThatShouldNotChange) {
    EXPECT_EQ(stem("cat"), "cat");
    EXPECT_EQ(stem("dog"), "dog");
}

// Test empty string input
TEST_F(StemmerTest, EmptyInput) {
    EXPECT_EQ(stem(""), "");
}

// Test very short words
TEST_F(StemmerTest, ShortWords) {
    EXPECT_EQ(stem("a"), "a");
    EXPECT_EQ(stem("it"), "it");
}
