// g++ -std=c++20 -Wall drill.cpp
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
  age 
   -1   (1)
    0   (1)
   11   (2)
   12   (3)
   16   (3)
   17   (4)
   18   (5)
   70   (5)
   71   (6)
  109   (6)
  110   (7)

With "Emma" as the name of the recipient and
"Sam" as the name of her/his friend, the first
three lines should be:

first_three_lines =
  "Dear Emma,\n"
  "    How are you? I am fine. I miss you.\n"
  "Have you seen Sam lately?\n"

Depending on the range the age, the variations
of the wording following the first three lines:

// (1) 0 or less,          age < 1
, output_0_or_less {
    first_three_lines +
    "you're kidding!\n"
}

// (2) under 12,      1 <= age < 12
, output_under_12 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Next year you will be _age_+1.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (3) (under 17),   12 <= age < 17
, output_under_17 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (4) if 17,              age == 17
, output_equal_17 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Next year you will be able to vote.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (5) (adult),      18 <= age < 71
, output_adult {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (6) over 70,      71 <= age < 110
, output_over_70 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Are you retired?\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (7) 110 or more, 110 <= age
, input_or_more_110 {
    first_three_lines +
    "you're kidding!\n"
}
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../doctest/doctest.h"
#include <utility>

const static std::string
  first_three_lines{
    "Dear Emma,\n"
    "    How are you? I am fine. I miss you.\n"
    "Have you seen Sam lately?\n"
  }

// (1) 0 or less,          age < 1
, output_0_or_less {
    first_three_lines +
    "you're kidding!\n"
}

// (2) under 12,      1 <= age < 12
, output_under_12 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Next year you will be _age_+1.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (3) (under 17),   12 <= age < 17
, output_under_17 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (4) if 17,              age == 17
, output_equal_17 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Next year you will be able to vote.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (5) (adult),      18 <= age < 71
, output_adult {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (6) over 70,      71 <= age < 110
, output_over_70 {
    first_three_lines +
    "I hear you just had a birthday"
    " and you are _age_ years old.\n"
    " Are you retired?\n"
    " Yours sincerely,\n\n\n"
    " Bjarne\n"
}

// (7) 110 or more, 110 <= age
, input_or_more_110 {
    first_three_lines +
    "you're kidding!\n"
}
;


