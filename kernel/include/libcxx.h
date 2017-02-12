#pragma once

#include <stddef.h>
#include <stdint.h>

#include <memory.h>
 
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *p);
void operator delete[](void *p);