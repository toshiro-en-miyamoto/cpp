#include <cstddef>
#include <array>
template <class T, std::size_t N>
class ring_buffer {
  std::array<T, N> arr;
  std::size_t write_idx = 0;
  std::size_t read_idx = 0;
  std::size_t count = 0;
public:
  bool is_empty() const {
    return count == 0;
  }
  void push(T value) {
    arr.at(write_idx) = value;
    write_idx = (write_idx + 1) % N;
    if (count < N) {
      count++;
    } else { // if buffer is full,
      // the count remains N which is the maximum number of elements,
      // and the read_idx moves forward to discard the oldest element
      read_idx = (read_idx + 1) % N;
    }
  }
  T pop() {
    if (count == 0) {
      return T{}; // return a default constructed T
    }
    T value = arr.at(read_idx);
    read_idx = (read_idx + 1) % N;
    --count;
    return value;
  }
};
