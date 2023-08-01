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

#include <sstream>
#include <boost/ut.hpp>

int main()
{

  using namespace boost::ut;

  "istream"_test = [&] {
    std::istringstream is(i_str);
    json j;
    is >> j;
    expect(3.141 == j["pi"]) << j["pi"];
    expect(true == j["happy"]) << j["happy"];
  };

  "ostream"_test = [&] {
    auto j = json::parse(i_str);
    std::ostringstream os;
    os << std::setw(2) << j <<std::endl;
    expect(os.str() == o_str) << os.str();
  };
}
