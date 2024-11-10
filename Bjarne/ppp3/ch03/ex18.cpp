// g++ -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion -Wno-psabi ex18.cpp
/*!
 * Enter a set of name-and-value pairs, such as `Joe 17` and `Barbara 22`.
 * For each pair, add the name to a vector called `names` and the number to a vector
 * called `scores` (in corresponding positions, so that if `names[7]=="Joe"` then
 * `scores[7]==17`). Terminate input with `NoName 0`.
 * Check that each name is unique and terminate with an error message if a name is
 * entered twice. Write out all the (name,score) pairs, one per line.
 */

#include <string>
#include <utility>

using Entry = std::pair<std::string, int>;

#include <optional>
#include <ranges>
#include <concepts>
#include <vector>

// Abbreviated function template
[[nodiscard]]
constexpr auto check(std::ranges::forward_range auto&& r)
-> std::optional<std::vector<Entry>>
requires std::same_as<std::ranges::range_value_t<decltype(r)>, Entry>
{
  std::vector<std::string> names;
  std::vector<int> scores;
  for (const auto& p : r) {
    if (p.first == "NoName") break;
    const auto it_name = std::ranges::find(names, p.first);
    if (it_name != names.end()) { // p.name found in names
      return std::nullopt;
    } else {
      names.push_back(p.first);
      scores.push_back(p.second);
    }
  }
  std::vector<Entry> entries;
  for (const auto& [name, score] : std::views::zip(names, scores)) {
    entries.emplace_back(std::make_pair(name, score));
  }
  return entries;
}

template <std::ranges::forward_range R>
  requires std::same_as<std::ranges::range_value_t<R>, Entry>
[[nodiscard]]
constexpr std::optional<std::vector<Entry>> check_dup(R&& r) noexcept
{
  std::vector<std::string> names;
  std::vector<int> scores;
  for (const auto& p : r) {
    if (p.first == "NoName") break;
    const auto it_name = std::ranges::find(names, p.first);
    if (it_name != names.end()) { // p.name found in names
      return std::nullopt;
    } else {
      names.push_back(p.first);
      scores.push_back(p.second);
    }
  }
  std::vector<Entry> entries;
  for (const auto& [name, score] : std::views::zip(names, scores)) {
    entries.emplace_back(std::make_pair(name, score));
  }
  return entries;
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_VOID_CAST_EXPRESSIONS    // for [[nodiscard]]
#include "../doctest/doctest.h"

#include <array>

constexpr std::array<Entry, 4> dataset1
{
  std::make_pair("Joe", 17),
  std::make_pair("Barbara", 22),
  std::make_pair("NoName", 0),
  std::make_pair("Eve", 0),
};

TEST_CASE("happy path")
{
  const std::vector<Entry> expected {
    std::make_pair("Joe", 17),
    std::make_pair("Barbara", 22),
  };
  const auto result = check(std::views::all(dataset1));
  CHECK_EQ(expected, result);
}

constexpr std::array<Entry, 4> dataset2
{
  std::make_pair("Joe", 17),
  std::make_pair("Barbara", 22),
  std::make_pair("Joe", 17),
  std::make_pair("Eve", 0),
};

TEST_CASE("duplicate name")
{
  REQUIRE_FALSE(check_dup(std::views::all(dataset2)));
}
