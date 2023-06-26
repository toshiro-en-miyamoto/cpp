#pragma once

#include <cstdio>
#include <memory>
#include <exception>

auto fcloser = [] (FILE* fp)
{ std::fclose(fp); };

using uniq_file_t = std::unique_ptr<FILE, decltype(fcloser)>;

struct file_handle_t : private uniq_file_t
{
  file_handle_t(const char* name, const char* mode)
    : uniq_file_t {fopen(name, mode), fcloser}
  {
    auto file = this->get();
    if (!file) throw std::invalid_argument(name);
  }

  using uniq_file_t::get;
};
