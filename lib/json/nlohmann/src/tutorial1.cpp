#include <sstream>

#include <boost/ut.hpp>
#include <nlohmann/json.hpp>

const char* s1 = R"(
{
  "pi": 3.141,
  "happy": true
}
)";

int main()
{
  using namespace boost::ut;
  using json = nlohmann::json;

  "istream"_test = [] {
    std::istringstream in1(s1);
    json j;
    in1 >> j;
    expect(3.141 == j["pi"]) << j["pi"];
    expect(true == j["happy"]) << j["happy"];
  };

  "ostream"_test = [] {
    json j1 {s1};
    std::stringstream out1;
    out1 << j1;
    json j2;
    out1 >> j2;
    expect(j1 == j2) << j1;
    expect(3.141 == j2["pi"]) << j2["pi"];
    expect(true == j2["happy"]) << j2["happy"];
  };
}
