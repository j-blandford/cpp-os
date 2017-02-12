#include <stdint.h>
#include <stddef.h>

#include <memory.h>

static void *kheap = &kernel_heap_start;
static const void *heap_start = &kernel_heap_start;

// memory allocation
void *malloc(size_t size) {
    kheap+=size;
    return kheap-size;
}

// re-allocate memory 
void *realloc(void * ptr, size_t size) { 
    if(ptr < heap_start)
    {
        return malloc(size);
    }
    else
    {
        void* newmem = malloc(size);
        memmove(newmem, ptr, size);
        free(ptr);
        return newmem;
    }
}


void *calloc(size_t num, size_t size) {
    size *= num;
    void* result = malloc(size);
    memset(result, 0, size);
    return result;
}

void free(void * ptr) {
    return; // meh for now - need to implement paging first!
}


int memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

void* memcpy(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, unsigned long size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (size_t i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

void* memset(void* bufptr, int value, size_t size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}
