#include <cstddef>
#include <array>
#define BUFFER_SIZE 5
class ring_buffer {
  std::array<int, BUFFER_SIZE> arr;
  std::size_t write_idx = 0;
  std::size_t read_idx = 0;
  std::size_t count = 0;
public:
  bool is_empty() const {
    return count == 0;
  }
  void push(int);
  int pop();
};
