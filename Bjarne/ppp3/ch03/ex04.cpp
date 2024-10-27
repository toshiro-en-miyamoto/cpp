// g++ -c -std=c++23 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wconversion ex04.cpp
/*
  Read a sequence of double values into a vector. Think of each value as the distance between two cities along a given route.
  - Compute the total distance (the sum of all distances).
  - Find the smallest and greatest distance between two neighboring cities.
  - Find the mean distance between two neighboring cities.
*/

#include <array>

constexpr std::array distances {2.9, 12.5, 7.4, 9.1, 0.6, 3.2};

#include <algorithm>
#include <functional>

namespace rng = std::ranges;
constexpr double total = rng::fold_left(distances, 0, std::plus<double>());
constexpr double smallest = rng::min(distances);
constexpr double greatest = rng::max(distances);
constexpr double mean = total / distances.size();

