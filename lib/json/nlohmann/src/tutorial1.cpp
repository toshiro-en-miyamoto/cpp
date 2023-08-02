#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char* i_str = R"({
  "pi": 3.141,
  "happy": true
}
)";

const char* o_str = R"({
  "happy": true,
  "pi": 3.141
}
)";

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <sstream>

TEST_CASE("istream") {
  std::istringstream is(i_str);
  json j;
  is >> j;
  CHECK(3.141 == j["pi"]);
  CHECK(true == j["happy"]);
};

TEST_CASE("ostream") {
  auto j = json::parse(i_str);
  std::ostringstream os;
  os << std::setw(2) << j <<std::endl;
  CHECK(os.str() == o_str);
};
