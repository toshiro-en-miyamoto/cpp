// clang++ --std=c++23 --assemble ring-buffer-clike.cpp ring-buffer-clike-impl.cpp
// clang++ --std=c++23 --output ring-buffer-clike.o ring-buffer-clike.cpp ring-buffer-clike-impl.cpp
// the size of ring-buffer-clike.o is 8,760 bytes
#include "ring-buffer-clike.h"
#include <stdio.h>
int main() {
  int_ring_buffer rb;
  int_ring_buffer_init(&rb);
  for (int i = 0; i < 7; i++) {
    int_ring_buffer_push(&rb, i);
  }
  while (rb.count > 0) {
    int value = int_ring_buffer_pop(&rb);
    printf("%d\n", value);
  }
}
