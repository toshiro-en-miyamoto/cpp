struct Point {
  int x, y, z;
};
#include <cstddef>
#include <utility>
Point expand(Point&& p) {
  p.x++;
  p.y++;
  p.z++;
  return p;
}
int main() {
  Point a{1, 2, 3};
  // an rvalue reference cannot bind to an lvalue
  // Point&& p cannot bind to Point a (a is an lvalue)
  // auto b = expand(a); --- compile error
  auto b = expand(std::move(a));
}
