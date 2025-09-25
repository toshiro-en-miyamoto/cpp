#include <stddef.h>
#define BUFFER_SIZE 5
typedef struct {
  int arr[BUFFER_SIZE];
  size_t write_idx;
  size_t read_idx;
  size_t count;
} int_ring_buffer;
void int_ring_buffer_init(int_ring_buffer*);
void int_ring_buffer_push(int_ring_buffer*, int);
int int_ring_buffer_pop(int_ring_buffer*);
