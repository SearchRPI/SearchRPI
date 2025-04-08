#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "snippets/highlighter.h"
#include "snippets/scorer.h"
#include "snippets/fragmenter.h"
#include "snippets/token.h"

using ::testing::Return;

//
// Highlighter tests
//

namespace Snippets {

// Test that the highlighter correctly wraps matching query terms with highlight markers.
TEST(HighlighterTest, HighlightsQueryTerms) {
    std::string text = "The quick brown fox jumps over the lazy dog.";
    std::set<std::string> queryTerms = {"fox", "dog"};

    // Create a scorer using the query terms.
    Scorer scorer(queryTerms);
    // Use a fragmenter that produces fragments of up to 50 characters.
    Fragmenter fragmenter(50);
    // Create a highlighter that wraps matches with <b> and </b>.
    Highlighter highlighter(&scorer, &fragmenter, "<b>", "</b>", "...");
    
    std::string bestFragment = highlighter.getBestFragment(text);

    // Check that the highlighted fragment contains our markers for "fox" and "dog".
    EXPECT_NE(bestFragment.find("<b>fox</b>"), std::string::npos);
    EXPECT_NE(bestFragment.find("<b>dog</b>"), std::string::npos);
}

// Test that when the document text is shorter than the fragment size, no truncation marker is appended.
TEST(HighlighterTest, ReturnsFullFragmentIfWithinBounds) {
    std::string text = "Hello world!";
    std::set<std::string> queryTerms = {"world"};
    
    Scorer scorer(queryTerms);
    // Set a fragment size large enough to include the entire text.
    Fragmenter fragmenter(100);
    Highlighter highlighter(&scorer, &fragmenter, "<em>", "</em>", "...");
    
    std::string fragment = highlighter.getBestFragment(text);
    // The word "world" should be highlighted.
    EXPECT_NE(fragment.find("<em>world</em>"), std::string::npos);
    // Since the text fits in one fragment, the omission marker ("...") should not appear.
    EXPECT_EQ(fragment.find("..."), std::string::npos);
}

//
// QueryScorer tests
//

TEST(ScorerTest, TokenScoring) {
    std::set<std::string> queryTerms = {"test"};
    Scorer scorer(queryTerms);
    scorer.startFragment();

    // Create two tokens: one that matches and one that does not.
    Token token1 = {"test", 0, 4};     // matches "test"
    Token token2 = {"example", 5, 12};   // does not match

    float score1 = scorer.getTokenScore(token1);
    float score2 = scorer.getTokenScore(token2);

    // Our simple scorer assigns a score equal to the token length for a match.
    EXPECT_FLOAT_EQ(score1, 4.0f);
    EXPECT_FLOAT_EQ(score2, 0.0f);
    // The total fragment score should equal the score from token1.
    EXPECT_FLOAT_EQ(scorer.getFragmentScore(), 4.0f);
}

//
// SimpleFragmenter tests
//

TEST(FragmenterTest, FragmentBoundaries) {
    // Create a fragmenter with a fragment size of 10 characters.
    Fragmenter fragmenter(10);
    std::string text = "abcdefghij klmnopqrst uvwxyz";
    // Manually create a set of tokens with known offsets.
    std::vector<Token> tokens{
        {"abcdefghij", 0, 10},
        {"klmnopqrst", 11, 21},
        {"uvwxyz", 22, 28}
    };
    fragmenter.start(text, tokens);
    
    // For the first token, its end equals 10 so should not trigger a new fragment.
    bool newFrag1 = fragmenter.isNewFragment(tokens[0]);
    // For the second token, its end (21) exceeds the first fragment boundary (10), so it should trigger a new fragment.
    bool newFrag2 = fragmenter.isNewFragment(tokens[1]);
    // For the third token, its end (28) should also trigger a new fragment.
    bool newFrag3 = fragmenter.isNewFragment(tokens[2]);

    EXPECT_FALSE(newFrag1);
    EXPECT_TRUE(newFrag2);
    EXPECT_TRUE(newFrag3);
}

} // namespace Snippets