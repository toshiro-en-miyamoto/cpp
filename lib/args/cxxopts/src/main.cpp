#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cxxopts.hpp>

TEST_CASE("basics") {
  cxxopts::Options options("example", "Example app with CxxOpts");
  options.add_options()
    ("d,debug", "Enable debugging") // a bool parameter
    ("i,integer", "Int param", cxxopts::value<int>())
    ("f,file", "File name", cxxopts::value<std::string>())
    (
      "v,verbose", "Verbose output",
      cxxopts::value<bool>()->default_value("false")
    );

  SUBCASE("") {
  }
}
