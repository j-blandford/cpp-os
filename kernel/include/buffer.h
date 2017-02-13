#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct buffer {
  size_t head;
  size_t tail;
  size_t size;
  uint8_t * buffer;
} buffer_t;

int buffer_read(buffer_t * buffer);
int buffer_write(buffer_t * buffer, uint8_t v);