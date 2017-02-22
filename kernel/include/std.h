#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <memory.h>

// #include <std/string.h>
// #include <std/vector.h>

// Structs for stdlib
typedef struct {
    short quot, rem;
} div_t;

typedef struct {
    long quot, rem;
} ldiv_t;

 
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *p);
void operator delete[](void *p);

// "String" functions
int strlen( char * ptr );
char *strcat(char *dest, const char *source);
int strncmp(const char *s1, const char *s2, size_t n);

void strncpy( char * ptr_dest, char * ptr_src, int n );

int putchar(int);

//int printf(const char* __restrict, ...);
int sprintf(char* buffer, const char* fmt, ...);
int vsprintf(char* buffer, const char* format, va_list parameters);

// Long-functions
ldiv_t ldiv (unsigned long numerator, unsigned long denominator );
char* ltoa(unsigned long long value, char* buffer, int radix );

// Integer functions
int itoa(int value, char *sp, int radix);
