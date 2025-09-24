// clang --std=c++20 --assemble raw-loop.cpp
#include <cstddef>
constexpr int buffer[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
constexpr std::size_t N = sizeof buffer;

int main() {
  int result = 0;
  for (int i = 0; i < N; i++) {
    result += buffer[i];
  }
}