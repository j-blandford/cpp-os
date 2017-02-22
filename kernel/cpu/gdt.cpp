#include <stddef.h>
#include <stdint.h>

#include <tty.h>

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

struct gdt_info gdt[3];
struct gdtr gp;

extern "C" void gdt_flush(struct gdtr* gp); // ASM function

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"

	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;

	#pragma GCC diagnostic pop
}

void gdt_install()
{
	gp.limit = (sizeof(struct gdt_info) * 3) - 1;
	gp.base = (uint32_t) &gdt;

    // flat 0-> 4GB memory mapping for ease. hardware segmentation is obsolete now
	gdt_set_gate(0, 0, 0, 0, 0); // first gate is ALWAYS null
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xC0); // 0x9A corresponds to ring 0 CODE memory segments
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xC0); // 0x92 corresponds to  ring 0 DATA memory segments
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xF8, 0xC0); // User Code (r3) segment maybe these 2 shouldn't overlap with the ring 0...
  	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xC0); // User Data (r3) segment
  //	gdt_set_gate(5, (uint32_t)&tss, sizeof(struct gdt_tss_entry), 0x89, 0x40); // TSS segment

    // to be included: TSS GDT gates.... watch this space

	gdt_flush(&gp); // flush the changes to the ASM function "gdt_flush" which calls the instruction LGDT and then performs a long jump to install the GDT
}