#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <hijack.hpp>
#include <iostream>

template <typename T>
struct Lifetime {
  T content;
  // default constructor
  Lifetime() : content{} {
    std::cout << "default constructed\n";
  }
  // value constructor
  Lifetime(const T& value) : content{value} {
    std::cout << "value constructed with " << content << '\n';
  }
  // copy constructor
  Lifetime(const Lifetime& other) : content{other.content} {
    std::cout << "copy constructed with " << other.content << '\n';
  }
  // copy assignment
  Lifetime& operator=(const Lifetime& other) {
    std::cout << "copy assigned with " << other.content << " over " << content << '\n';
    content = other.content;
    return *this;
  }
  // move constructor
  Lifetime(Lifetime &&other) : content{other.content} {
    std::cout << "move constructed with " << other.content << '\n';
    other.content = T{};
  }
  // move assignment
  Lifetime& operator=(Lifetime &&other) {
    std::cout << "move assigned with " << other.content << " over " << content << '\n';
    std::swap(other.content, content);
    return *this;
  }
  // destructor
  ~Lifetime() {
    std::cout << "destructing " << content << '\n'; 
  }
};

TEST_CASE("default constructed") {
  my::Hijack out(std::cout);
  {
    Lifetime<int> life;
  }
  const auto message{out.release()};
  const std::string expected{
    "default constructed\n"
    "destructing 0\n"
  };
  CHECK(message == expected);
}
TEST_CASE("rvalue constructed") {
  my::Hijack out(std::cout);
  {
    Lifetime life{1};
  }
  const auto message{out.release()};
  const std::string expected{
    "value constructed with 1\n"
    "destructing 1\n"
  };
  CHECK(message == expected);
}
TEST_CASE("copy constructed") {
  my::Hijack out(std::cout);
  {
    Lifetime life1{1};
    life1.content++;
    Lifetime life2{life1};
  }
  const auto message{out.release()};
  const std::string expected{
    "value constructed with 1\n"
    "copy constructed with 2\n"
    "destructing 2\n"
    "destructing 2\n"
  };
  CHECK(message == expected);
}
TEST_CASE("move constructed") {
  my::Hijack out(std::cout);
  {
    Lifetime life1{1};
    Lifetime life2{std::move(life1)};
  }
  const auto message{out.release()};
  const std::string expected{
    "value constructed with 1\n"
    "move constructed with 1\n"
    "destructing 1\n"
    "destructing 0\n"
  };
  CHECK(message == expected);
}
TEST_CASE("copy assigned") {
  my::Hijack out(std::cout);
  {
    Lifetime<int> life1;
    Lifetime life2{1};
    life1 = life2;
  }
  const auto message{out.release()};
  const std::string expected{
    "default constructed\n"
    "value constructed with 1\n"
    "copy assigned with 1 over 0\n"
    "destructing 1\n"
    "destructing 1\n"
  };
  CHECK(message == expected);
}
TEST_CASE("move assigned") {
  my::Hijack out(std::cout);
  {
    Lifetime<int> life1;
    Lifetime life2{1};
    life1 = std::move(life2);
  }
  const auto message{out.release()};
  const std::string expected{
    "default constructed\n"
    "value constructed with 1\n"
    "move assigned with 1 over 0\n"
    "destructing 0\n"
    "destructing 1\n"
  };
  CHECK(message == expected);
}
