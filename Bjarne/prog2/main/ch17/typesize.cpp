#include <ch17/typesize.h>

namespace my {

template<typename T>
size_pair typesize(T val)
{
    return std::make_pair(sizeof(T), sizeof(val));
}

template size_pair typesize <char> (char);
template size_pair typesize <int>  (int);
template size_pair typesize <long> (long);
template size_pair typesize <int*> (int*);
template size_pair typesize <bool> (bool);
template size_pair typesize <double> (double);

}
