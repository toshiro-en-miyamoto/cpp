#include <deque>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <boost/ut.hpp>

int main()
{
  using namespace boost;
  using namespace boost::ut;

  "push"_test = []
  {
    std::deque deque1{2, 3};
    deque1.push_front(1);
    deque1.push_front(0);
    deque1.push_back(4);
    ut::expect(deque1 == std::deque{0, 1, 2, 3, 4});
  };
  "pop"_test = []
  {
    std::deque deque1{0, 1, 2, 3, 4};
    deque1.pop_front();
    deque1.pop_front();
    ut::expect(deque1 == std::deque{2, 3, 4});
  };
  "copy with back_inserter"_test = []
  {
    std::deque deque1{0, 1, 2, 3, 4};
    std::deque<int> deque2;
    std::ranges::copy(std::as_const(deque1), std::back_inserter(deque2));
    ut::expect(deque1 == deque2);
  };
  "copy by assignment"_test = []
  {
    std::deque deque1{0, 1, 2, 3, 4};
    std::deque deque2 = deque1;
    ut::expect(deque1 == deque2);
  };
  "copy removing consecutive duplicates"_test = []
  {
    std::deque deque1{0, 1, 1, 2, 3};
    std::deque<int> deque2;
    std::ranges::unique_copy(std::as_const(deque1), std::back_inserter(deque2));
    ut::expect(deque2 == std::deque{0, 1, 2, 3});
  };
  return 0;
}
