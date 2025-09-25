# Ring Buffer

Pushing elements:

```c
void int_ring_buffer_push(int_ring_buffer* rb, int value) {
  rb->arr[rb->write_idx] = value;
  rb->write_idx = (rb->write_idx + 1) % BUFFER_SIZE;
  if (rb->count < BUFFER_SIZE) {
    rb->count++;
  } else {
    rb->read_idx = (rb->read_idx + 1) % BUFFER_SIZE;
  }
}
```

| i | w_idx | r_idx | cnt | buffer |
|--:|------:|------:|----:|:-------|
|   | 0     | 0     | 0   | _____  |
| 0 | 1     | 0     | 1   | 0____  |
| 1 | 2     | 0     | 2   | 01___  |
| 2 | 3     | 0     | 3   | 012__  |
| 3 | 4     | 0     | 4   | 0123_  |
| 4 | 0     | 0     | 5   | 01234  |
| 5 | 1     | 1     | 5   | 51234  |
| 6 | 2     | 2     | 5   | 56234  |

Popping elements:

```c
int int_ring_buffer_pop(int_ring_buffer* rb) {
  if (rb->count == 0) {
    return 0;
  }
  int value = rb->arr[rb->read_idx];
  rb->read_idx = (rb->read_idx + 1) % BUFFER_SIZE;
  rb->count--;
  return value;
}
```

| i | w_idx | value | r_idx | cnt |
|--:|------:|------:|------:|----:|
|   | 2     |       | 2     | 5   |
| - | 2     | 2     | 3     | 4   |
| - | 2     | 3     | 4     | 3   |
| - | 2     | 4     | 0     | 2   |
| - | 2     | 5     | 1     | 1   |
| - | 2     | 6     | 2     | 0   |

