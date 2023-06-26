#include <boost/ut.hpp>
#include "./entity.h"
#include <functional>
#include <string_view>
#include <sstream>
#include <iostream>

int main()
{
  std::ostringstream str_out;
  auto cout_buf = std::cout.rdbuf();
  std::cout.rdbuf(str_out.rdbuf());

  {
    Entity e1 {"foo"};
    e1 = f(e1);
    Entity e2 {"bar"};
    e1 = e2;
  }

  std::cout.rdbuf(cout_buf);
  std::string_view expected {
    "ctor: foo\n"
    "copy ctor: foo\n"
    "move ctor: foo\n"
    "move assign: foo\n"
    "dtor: \n"
    "dtor: \n"
    "ctor: bar\n"
    "copy assign: bar\n"
    "dtor: bar\n"
    "dtor: bar\n"
  };

  using namespace boost::ext::ut;
  expect(that % str_out.str() == expected);

  return 0;
}
