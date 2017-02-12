#pragma once

#include <stddef.h>

struct gdt_info {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__ ((packed));

struct gdtr {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));

extern struct gdt_info* gdt;
extern struct gdtr gp;

extern void gdt_flush(void);

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();