#include <stddef.h> 
#include <stdint.h> 

#include <std.h>

#include <tty.h>

#include <cpu/multiboot.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>

#include <gfx/vesa.h>
#include <gfx/surface.h>

#include <devices/keyboard.h>
#include <devices/ata.h>

#include <drivers/pci.h>

extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t stack_size, uintptr_t esp) {
	interrupts_disable();
	terminal_initialize(); 

	init_fbe(mb_info);
	init_screens();

	init_pci();
	init_ata();

	terminal_printf("total memory:  %d kb\n",mb_info->mem_upper + mb_info->mem_lower);
	terminal_printf("vga:  %dx%d (%d) \n",frame_width, frame_height, frame_pitch);

	//test_surfaces();

	update_buffer();

	gdt_install();
	idt_install();

	keyboard_install();

	interrupts_enable();

	char buffer[1024];

	while (true) {
		terminal_writestring("kernel", RGBA(0xDDDDDD));
		terminal_writestring("> ", RGBA(0xFFFFFF));
		update_buffer();

		getsn(&buffer[0], 1024);

	}
}