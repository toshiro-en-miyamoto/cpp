// g++ type_size_ut.cpp type_size.cpp -std=c++20
#include "type_size.h"
#include <boost/ut.hpp>

int main()
{
  using namespace boost::ut;

  suite type_size_ut = []
  {
    "size of char"_test = []
    {
      auto [sz_type, sz_value] = type_size<char>('a');
      expect(sz_type == 1_i);
      expect(sz_value == 1_i);
    };
    "size of int"_test = []
    {
      auto [sz_type, sz_value] = type_size<int>(1);
      expect(sz_type == 4_i);
      expect(sz_value == 4_i);
    };
    "size of long"_test = []
    {
      auto [sz_type, sz_value] = type_size<long>(1L);
      expect(sz_type == 8_i);
      expect(sz_value == 8_i);
    };
    "size of pointer to int"_test = []
    {
      int i = 1;
      int *pi = &i;
      auto [sz_type, sz_value] = type_size<int *>(pi);
      expect(sz_type == 8_i);
      expect(sz_value == 8_i);
    };
    "size of bool"_test = []
    {
      auto [sz_type, sz_value] = type_size<bool>(true);
      expect(sz_type == 1_i);
      expect(sz_value == 1_i);
    };
    "size of double"_test = []
    {
      auto [sz_type, sz_value] = type_size<double>(1);
      expect(sz_type == 8_i);
      expect(sz_value == 8_i);
    };
  };

  suite type_size2_ut = []
  {
    "size of char"_test = []
    {
      auto sz_type = type_size2('a');
      expect(sz_type == 1_i);
    };
    "size of int"_test = []
    {
      auto sz_type = type_size2(1);
      expect(sz_type == 4_i);
    };
    "size of long"_test = []
    {
      auto sz_type = type_size2(1L);
      expect(sz_type == 8_i);
    };
    "size of pointer to int"_test = []
    {
      int i = 1;
      int *pi = &i;
      auto sz_type = type_size2(pi);
      expect(sz_type == 8_i);
    };
    "size of bool"_test = []
    {
      auto sz_type = type_size2(true);
      expect(sz_type == 1_i);
    };
    "size of double"_test = []
    {
      auto sz_type = type_size2<double>(1);
      expect(sz_type == 8_i);
    };
  };
}
