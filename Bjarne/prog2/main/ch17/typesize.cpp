#include <ch17/typesize.h>

namespace ch17 {

template<typename T>
size_pair typesize(T val)
{
    return std::make_pair(sizeof(T), sizeof(val));
}

template size_pair typesize(char);
template size_pair typesize(int);
template size_pair typesize(long);
template size_pair typesize(int*);
template size_pair typesize(bool);
template size_pair typesize(double);

size_t typesize2(auto val)
{
    return sizeof(decltype(val));
}

template size_t typesize2(char);
template size_t typesize2(int);
template size_t typesize2(long);
template size_t typesize2(int*);
template size_t typesize2(bool);
template size_t typesize2(double);

}
