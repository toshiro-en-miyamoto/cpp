#pragma once

#include <cstddef>
#include <utility>

using size_pair = std::pair<std::size_t, std::size_t>;

template<typename T> size_pair type_size(T val);

extern template size_pair type_size(char);
extern template size_pair type_size(int);
extern template size_pair type_size(long);
extern template size_pair type_size(int*);
extern template size_pair type_size(bool);
extern template size_pair type_size(double);

size_t type_size2(auto val);

extern template size_t type_size2(char);
extern template size_t type_size2(int);
extern template size_t type_size2(long);
extern template size_t type_size2(int*);
extern template size_t type_size2(bool);
extern template size_t type_size2(double);
