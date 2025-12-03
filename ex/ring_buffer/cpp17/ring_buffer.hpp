/**
 * @brief Ring_buffer is a FIFO container that can store at most N
 * elements of type T.
 * 
 * Ring_buffer stores its elements directly within the Ring_buffer
 * object itself. It means that should a Ring_buffer be declared as
 * a local variable, its elements are allocated on the stack.
 * 
 * Multi-thread support is ???
 * 
 * The `push()` method of a Ring_buffer object copies/moves the
 * argument into a buffer built in the object. If the buffer is full,
 * the method will not wait but overwrite the oldest element in the
 * buffer with the argument.
 * 
 * The `pop()` method may return the oldest element in the buffer. The
 * returned value will be virtually removed from the buffer. If the buffer
 * is empty, `optional.empty` is returned.
 */
#include <cstddef>
#include <optional>
#include <array>

namespace my {

/**
 * FIFO containers that can store at most N elements of type T.
 * 
 * @tparam T The element stored in a built-in buffer.
 * @tparam N The maximum number of elements in the buffer.
 */
template<typename T, std::size_t N>
class Ring_buffer {
public:
  /**
   * Copies the argument into a buffer built in the instance.
   * 
   * @param T The element to be copied into the buffer.
   */
  void push(const T&);

  /**
   * Moves the argument into a buffer built in the instance.
   * 
   * @param T The element to be moved into the buffer.
   */
  void push(T&&);

  /**
   * Returns the oldest element in the buffer.
   * 
   * @return The optional value of the oldest element in the
   * buffer, or `optional.empty`.
   */
  std::optional<T> pop();

  private:
  std::array<T, N> buffer{};
  std::size_t write_idx{0};
  std::size_t read_idx{0};
  std::size_t count{0};
};

template<typename T, std::size_t N>
void Ring_buffer<T, N>::push(const T& e) {
  buffer[write_idx] = e;
  write_idx = (write_idx + 1) % N;
  if (count < N) {
    count++;
  } else {
    read_idx = (read_idx + 1) % N;
  }
}

template<typename T, std::size_t N>
void Ring_buffer<T, N>::push(T&& e) {
  std::swap(buffer[write_idx], e);
  write_idx = (write_idx + 1) % N;
  if (count < N) {
    count++;
  } else {
    read_idx = (read_idx + 1) % N;
  }
}

template<typename T, std::size_t N>
std::optional<T> Ring_buffer<T, N>::pop() {
  if (count == 0) {
    return {};
  }
  auto idx = read_idx;
  read_idx = (read_idx + 1) % N;
  count--;

  return std::move(buffer[idx]);
}

}
