#pragma once

#include <cstddef>
#include <utility>

namespace my {

template<typename T>
auto typesize(T val) -> const std::pair<std::size_t, std::size_t>
{
    return std::make_pair(sizeof(T), sizeof(val));
}

}
