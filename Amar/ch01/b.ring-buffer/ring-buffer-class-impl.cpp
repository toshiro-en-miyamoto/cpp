#include "ring-buffer-class.h"
void ring_buffer::push(int value) {
  arr.at(write_idx) = value;
  write_idx = (write_idx + 1) % BUFFER_SIZE;
  if (count < BUFFER_SIZE) {
    count++;
  } else { // if buffer is full,
    // the count remains BUFFER_SIZE which is the maximum number of elements,
    // and the read_idx moves forward to discard the oldest element
    read_idx = (read_idx + 1) % BUFFER_SIZE;
  }
}
int ring_buffer::pop() {
  if (count == 0) {
    return 0; // return a default constructed int
  }
  int value = arr.at(read_idx);
  read_idx = (read_idx + 1) % BUFFER_SIZE;
  --count;
  return value;
}
