#pragma once

#include <ostream>
#include <sstream>

namespace my {

/**
 * Redirect a stream characters from a given std:ostream to the
 * built-in std:stringstream until released or destructed.
 * The release() method returns a std::string, which is a copy
 * of what the user has given to the redirected stream.
 *
 * sample usage:
 *     my::hijack hj(std::cout);
 *     std::cout << "Hello";
 *     std::cout << hj.release().append(" world!\n");
 *     // this prints "Hello world!\n" to std::cout
 */
class Hijack
{
public:
  explicit Hijack(std::ostream &os)
  : original_ostream_{os}, original_rdbuf_{os.rdbuf()}
  {
    original_ostream_.rdbuf(redirected_ostream_.rdbuf());
  }

  ~Hijack()
  {
    original_ostream_.rdbuf(original_rdbuf_);
  }

  std::string release()
  {
    original_ostream_.rdbuf(original_rdbuf_);
    return redirected_ostream_.str();
  }

private:
  std::ostream &original_ostream_;
  std::streambuf* original_rdbuf_;
  std::ostringstream redirected_ostream_{};
};

}
