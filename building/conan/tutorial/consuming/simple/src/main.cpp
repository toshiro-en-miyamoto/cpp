#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <array>
#include <string>
#include <algorithm>
#include <functional>
#include <ranges>
#include <iostream>

#include <zlib.h>

int main(void) {
  constexpr size_t BUF_LEN = 256;

  std::array<char, BUF_LEN> out {};
  std::array<char, BUF_LEN> in {
    "Conan is a MIT-licensed, Open Source package "
    "manager for C and C++ development for C and C++ "
    "development, allowing development teams to easily "
    "and efficiently manage their packages and "
    "dependencies across platforms and build systems."
  };

  const auto IN_LEN {
    std::ranges::count_if(in, [](char c){ return c != 0; })
  };

  z_stream defstream;
  defstream.zalloc = Z_NULL;
  defstream.zfree = Z_NULL;
  defstream.opaque = Z_NULL;
  defstream.avail_in = IN_LEN;
  defstream.next_in = reinterpret_cast<Byte*>(in.data());
  defstream.avail_out = out.size();
  defstream.next_out = reinterpret_cast<Byte*>(out.data());

  deflateInit(&defstream, Z_BEST_COMPRESSION);
  deflate(&defstream, Z_FINISH);
  deflateEnd(&defstream);

  const auto OUT_LEN {
    std::ranges::count_if(out, [](char c){ return c != 0; })
  };

  std::cout << "Uncompressed size is: " << IN_LEN << std::endl;
  std::cout << "Compressed size is: " << OUT_LEN << std::endl;

  std::cout
  << "ZLIB VERSION: " << zlibVersion() << std::endl;
}