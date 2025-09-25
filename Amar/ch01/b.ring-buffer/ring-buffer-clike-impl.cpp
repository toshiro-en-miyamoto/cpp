#include "ring-buffer-clike.h"
void int_ring_buffer_init(int_ring_buffer* rb) {
  rb->write_idx = 0;
  rb->read_idx = 0;
  rb->count = 0;
}
void int_ring_buffer_push(int_ring_buffer* rb, int value) {
  rb->arr[rb->write_idx] = value;
  rb->write_idx = (rb->write_idx + 1) % BUFFER_SIZE;
  if (rb->count < BUFFER_SIZE) {
    rb->count++;
  } else {
    rb->read_idx = (rb->read_idx + 1) % BUFFER_SIZE;
  }
}
int int_ring_buffer_pop(int_ring_buffer* rb) {
  if (rb->count == 0) {
    return 0;
  }
  int value = rb->arr[rb->read_idx];
  rb->read_idx = (rb->read_idx + 1) % BUFFER_SIZE;
  rb->count--;
  return value;
}
