#pragma once

#include <cstddef>

namespace my {

/**
 * Searches an array of char elements (data[] of sz elements) for a char (val).
 * 
 * pre-conditions:
 * - Elements of the array shall be sorted by '<' (less than).
 * - 0 < sz < std::numeric_limits<std::size_t>::max().
 * post-conditions:
 * - none
 * returns:
 * - an index to point to an element in data[] if val is found, or
 *   the same value as sz otherwise
 */
std::size_t binary_search(const char data[], std::size_t sz, char val)
{
  // searching the range [begin, end)
  std::size_t begin = 0;
  std::size_t end = sz;
  std::size_t i;        // current middle element

  while (begin < end) {
    i = (begin + end - 1)/2;
    if (data[i] < val)
      begin = i + 1;
    else if (val < data[i])
      end = i;
    else                // data[i] == val
      return i;
  }

  return sz;            // if val is not found in data[]
}

}
