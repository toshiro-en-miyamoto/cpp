#pragma once

#include <cstddef>
#include <utility>

namespace ch17 {

using size_pair = std::pair<std::size_t, std::size_t>;

template<typename T> size_pair typesize(T val);

extern template size_pair typesize(char);
extern template size_pair typesize(int);
extern template size_pair typesize(long);
extern template size_pair typesize(int*);
extern template size_pair typesize(bool);
extern template size_pair typesize(double);

size_t typesize2(auto val);

extern template size_t typesize2(char);
extern template size_t typesize2(int);
extern template size_t typesize2(long);
extern template size_t typesize2(int*);
extern template size_t typesize2(bool);
extern template size_t typesize2(double);
}
