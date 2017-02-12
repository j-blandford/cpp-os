#pragma once

#include <stddef.h>
#include <stdint.h>

extern int kernel_heap_start;

// Memory Functions
int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, unsigned long);
void* memset(void*, int, size_t);


// will replace with liballoc later.
// put the most simple implementation in for now

void     *malloc(size_t);
void     *realloc(void *, size_t);
void     *calloc(size_t, size_t);
void      free(void *);

