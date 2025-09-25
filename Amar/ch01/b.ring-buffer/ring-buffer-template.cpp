// clang++ --std=c++23 --assemble ring-buffer-template.cpp
// clang++ --std=c++23 --output ring-buffer-template.o ring-buffer-template.cpp
// the size of ring-buffer-template.o is 9,808 bytes
#include "ring-buffer-template.h"
#include <stdio.h>
int main() {
  ring_buffer<int, 5> rb;
  for (int i = 0; i < 7; ++i) {
    rb.push(i);
  }
  while(!rb.is_empty()) {
    int value = rb.pop();
    printf("%d\n", value);
  }
}
