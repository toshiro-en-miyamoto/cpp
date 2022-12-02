#pragma once

#include <vector>
#include <concepts>

namespace ch19
{

template<std::signed_integral T>
void add(std::vector<T>& v1, const std::vector<T>& v2)
{
    auto sz1 {v1.size()}, sz2 {v2.size()};
    auto sz = sz1 < sz2 ? sz1 : sz2;
    for (auto i = 0; i < sz; ++i)
        v1[i] += v2[i];
};

template<std::signed_integral T, std::floating_point U>
U product(const std::vector<T>& vt, const std::vector<U>& vu)
{
    auto vt_sz {vt.size()}, vu_sz {vu.size()};
    auto sz = vt_sz < vu_sz ? vt_sz : vu_sz;
    U sum = 0;
    for (auto i = 0; i < sz; ++i)
        sum += vt[i] * vu[i];
    return sum;
}

}
