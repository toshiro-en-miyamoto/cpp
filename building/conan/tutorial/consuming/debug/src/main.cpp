#include <zlib.h>

#include <vector>
#include <cstddef>
#include <string>
#include <ranges>
#include <iterator>

namespace my
{
  std::vector<std::byte> compress(std::u8string&& in)
  {
    const auto BUF_SIZE = in.size();
    std::vector<std::byte> out(BUF_SIZE);

    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = BUF_SIZE;
    defstream.next_in = reinterpret_cast<Bytef*>(in.data());
    defstream.avail_out = BUF_SIZE;
    defstream.next_out = reinterpret_cast<Bytef*>(out.data());

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    const auto OUT_SIZE
    = std::ranges::distance(out.begin(), std::ranges::find(out, std::byte{0x00}));
    out.resize(OUT_SIZE);

    return out;
  }
}

#include <iostream>

int main()
{
  static std::u8string in {
    u8"Conan is a MIT-licensed, Open Source package "
    "manager for C and C++ development for C and C++ "
    "development, allowing development teams to easily "
    "and efficiently manage their packages and "
    "dependencies across platforms and build systems."
  };
  std::cout
  << "Uncompressed size is: " << in.size() << std::endl;

  auto out = my::compress(std::move(in));
  
  std::cout
  << "Compressed size is: " << out.size() << std::endl;
  std::cout
  << "ZLIB VERSION: " << zlibVersion() << std::endl;

  std::cout << 
  #ifdef NDEBUG
  "Release configuration!\n";
  #else
  "Debug configuration!\n";
  #endif
}
