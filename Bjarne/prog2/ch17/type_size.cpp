#include "type_size.h"

template<typename T>
size_pair type_size(T val)
{
    return std::make_pair(sizeof(T), sizeof(val));
}

template size_pair type_size(char);
template size_pair type_size(int);
template size_pair type_size(long);
template size_pair type_size(int*);
template size_pair type_size(bool);
template size_pair type_size(double);

size_t type_size2(auto val)
{
    return sizeof(decltype(val));
}

template size_t type_size2(char);
template size_t type_size2(int);
template size_t type_size2(long);
template size_t type_size2(int*);
template size_t type_size2(bool);
template size_t type_size2(double);
