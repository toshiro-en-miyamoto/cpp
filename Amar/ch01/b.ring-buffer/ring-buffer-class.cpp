// clang++ --std=c++23 --assemble ring-buffer-class.cpp ring-buffer-class-impl.cpp
// clang++ --std=c++23 --output ring-buffer-class.o ring-buffer-class.cpp ring-buffer-class-impl.cpp
// the size of ring-buffer-class.o is 9,664 bytes
#include "ring-buffer-class.h"
#include <stdio.h>
int main() {
  ring_buffer rb;
  for (int i = 0; i < 7; ++i) {
    rb.push(i);
  }
  while(!rb.is_empty()) {
    int value = rb.pop();
    printf("%d\n", value);
  }
}
