// g++ -std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion drill.cpp
/*
Produce a simple form letter based on input.
The wording of the letter changes depending
on the age of the recipient.

The ranges of the age determine the wording:
(1) 0 or less,          age < 1
(2) under 12,      1 <= age < 12
(3) (under 17),   12 <= age < 17
(4) if 17,              age == 17
(5) (adult),      18 <= age < 71
(6) over 70,      71 <= age < 110
(7) 110 or more, 110 <= age

Test cases:
  TC# age   range
  01   -1   (1) 0 or less,          age < 1
  02    0   (1) 0 or less,          age < 1
  03    1   (2) under 12,      1 <= age < 12
  04   11   (2) under 12,      1 <= age < 12
  05   12   (3) (under 17),   12 <= age < 17
  06   16   (3) (under 17),   12 <= age < 17
  07   17   (4) if 17,              age == 17
  08   18   (5) (adult),      18 <= age < 71
  09   70   (5) (adult),      18 <= age < 71
  10   71   (6) over 70,      71 <= age < 110
  11  109   (6) over 70,      71 <= age < 110
  12  110   (7) 110 or more, 110 <= age

In out test cases, we call "Emma" as the recipient
of the letter and "Sam" as her friend.
*/

#include <string_view>
using namespace std::literals;

struct Test_case {
  const std::string_view input;
  const std::string_view expected;
};

constexpr static Test_case tc[]{
  {
    //  TC# age   range
    //  01   -1   (1) 0 or less,          age < 1
    "Emma Sam -1",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "you're kidding!\n"sv
  }, {
    //  TC# age   range
    //  02    0   (1) 0 or less,          age < 1
    "Emma Sam 0",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "you're kidding!\n"
  }, {
    //  TC# age   range
    //  03    1   (2) under 12,      1 <= age < 12
    "Emma Sam 1",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 1 years old.\n"
      "Next year you will be 2.\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  04   11   (2) under 12,      1 <= age < 12
    "Emma Sam 11",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 11 years old.\n"
      "Next year you will be 12.\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  05   12   (3) (under 17),   12 <= age < 17
    "Emma Sam 12",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 12 years old.\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  06   16   (3) (under 17),   12 <= age < 17
    "Emma Sam 16",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 16 years old.\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  07   17   (4) if 17,              age == 17
    "Emma Sam 17",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 17 years old.\n"
      "Next year you will be able to vote.\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  08   18   (5) (adult),      18 <= age < 71
    "Emma Sam 18",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 18 years old.\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  09   70   (5) (adult),      18 <= age < 71
    "Emma Sam 70",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 70 years old.\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  10   71   (6) over 70,      71 <= age < 110
    "Emma Sam 71",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 71 years old.\n"
      "Are you retired?\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  11  109   (6) over 70,      71 <= age < 110
    "Emma Sam 109",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "I hear you just had a birthday"
      " and you are 109 years old.\n"
      "Are you retired?\n"
      "Yours sincerely,\n\n\n"
      "Bjarne\n"
  }, {
    //  TC# age   range
    //  12  110   (7) 110 or more, 110 <= age
    "Emma Sam 110",
    "Dear Emma,\n"
      "\tHow are you? I am fine. I miss you.\n"
      "Have you seen Sam lately?\n"
      "you're kidding!\n"
  }
};

// Produce the letter content
#include <sstream>
#include <format>

// The letter wording templates
constexpr static std::string_view
/*
    description   age range
(1) 0 or less,          age < 1
(7) 110 or more, 110 <= age
*/
fmt_kidding {
  "Dear {},\n"                    // name
  "\tHow are you? I am fine. I miss you.\n"
  "Have you seen {} lately?\n"    // friend name
  "you're kidding!\n"
}
/*
    description   age range
(2) under 12,      1 <= age < 12
*/
, fmt_under_12 {
  "Dear {},\n"                    // name
  "\tHow are you? I am fine. I miss you.\n"
  "Have you seen {} lately?\n"    // friend name
  "I hear you just had a birthday"
  " and you are {} years old.\n"  // age
  "Next year you will be {}.\n"   // age+1
  "Yours sincerely,\n\n\n"
  "Bjarne\n"
}
/*
    description   age range
(3) (under 17),   12 <= age < 17
(5) (adult),      18 <= age < 71
*/
, fmt_default {
  "Dear {},\n"                    // name
  "\tHow are you? I am fine. I miss you.\n"
  "Have you seen {} lately?\n"    // friend name
  "I hear you just had a birthday"
  " and you are {} years old.\n"  // age
  "Yours sincerely,\n\n\n"
  "Bjarne\n"
}
/*
    description   age range
(4) if 17,              age == 17
*/
, fmt_17 {
  "Dear {},\n"                    // name
  "\tHow are you? I am fine. I miss you.\n"
  "Have you seen {} lately?\n"    // friend name
  "I hear you just had a birthday"
  " and you are {} years old.\n"  // age
  "Next year you will be able to vote.\n"
  "Yours sincerely,\n\n\n"
  "Bjarne\n"
}
/*
    description   age range
(6) over 70,      71 <= age < 110
*/
, fmt_retired {
  "Dear {},\n"                    // name
  "\tHow are you? I am fine. I miss you.\n"
  "Have you seen {} lately?\n"    // friend name
  "I hear you just had a birthday"
  " and you are {} years old.\n"  // age
  "Are you retired?\n"
  "Yours sincerely,\n\n\n"
  "Bjarne\n"
}
;

void letter_stream(std::istream& is, std::ostream& os)
{
  std::string name, friend_name;
  int age{0};

  is >> name >> friend_name >> age;

  if (age < 1)
    os << std::format(fmt_kidding, name, friend_name);
  else if (age < 12)
    os << std::format(fmt_under_12, name, friend_name, age, age+1);
  else if (age < 17)
    os << std::format(fmt_default, name, friend_name, age);
  else if (age == 17)
    os << std::format(fmt_17, name, friend_name, age);
  else if (age < 71)
    os << std::format(fmt_default, name, friend_name, age);
  else if (age < 110)
    os << std::format(fmt_retired, name, friend_name, age);
  else
    os << std::format(fmt_kidding, name, friend_name);
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"

[[nodiscard]]
std::string report_string(const std::string_view input)
{
  std::istringstream is{std::string{input}};
  std::ostringstream os;

  letter_stream(is, os);

  return os.str();
}

TEST_CASE("12 test cases")
{
  CHECK(tc[0].expected == report_string(tc[0].input));
  CHECK(tc[1].expected == report_string(tc[1].input));
  CHECK(tc[2].expected == report_string(tc[2].input));
  CHECK(tc[3].expected == report_string(tc[3].input));
  CHECK(tc[4].expected == report_string(tc[4].input));
  CHECK(tc[5].expected == report_string(tc[5].input));
  CHECK(tc[6].expected == report_string(tc[6].input));
  CHECK(tc[7].expected == report_string(tc[7].input));
  CHECK(tc[8].expected == report_string(tc[8].input));
  CHECK(tc[9].expected == report_string(tc[9].input));
  CHECK(tc[10].expected == report_string(tc[10].input));
  CHECK(tc[11].expected == report_string(tc[11].input));
}
