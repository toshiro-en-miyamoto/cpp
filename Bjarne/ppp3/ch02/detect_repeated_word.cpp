/* g++ -std=c++20 -Wall detect_repeated_word.cpp
This is a "Try This" in 2.5.1 An example: detect repeated word.
This program detects adjacent repeated word in a sequence of word. 
*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <sstream>

const std::string text1{"The cat cat jumped"};
const std::string text2{
  "She she laughed \"he he he!\" because"
  " what he did did not look very good good"
};

[[nodiscard]]
int number_of_repeated_words_in(const std::string text)
{
  std::stringstream is{text};
  std::string current;
  std::string previous;
  int counter = 0;

  while(is >> current) {
    if (current == previous)
      counter++;
    previous = current;
  }
  return counter;
}

TEST_CASE("count the number of adjacent repeated word in the text")
{
  CHECK(number_of_repeated_words_in(text1) == 1);

  // two occurrences of adjacent repeated word in text2,
  //  "did" == "did", "good" == "good"
  //  "\"he" != "he", "he" != "he!\""
  CHECK(number_of_repeated_words_in(text2) == 2);
}

[[nodiscard]]
int number_of_words_in(const std::string text)
{
  std::stringstream is{text};
  std::string current;
  int counter = 0;

  while(is >> current) {
    counter++;
  }
  return counter;
}

TEST_CASE("count the number of words in the text")
{
  CHECK(number_of_words_in(text1) == 4);

  // for a string the operator >> reads whitespace-separated words
  // hence "\"he" is a word and so is "he!\""
  CHECK(number_of_words_in(text2) == 16);
}

