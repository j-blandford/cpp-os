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


void init_screens() {
	std::vector<Surface> screen_surfaces;
	screen_surfaces.push_back(Surface(Vector2(50,250), Vector2(50,50)));

	screen_surfaces[0].setBackground(RGBA(0xFFFFFF));
	screen_surfaces[0].drawCircle(25, 25, 20, RGBA(0xFF0000));
	screen_surfaces[0].apply();

}


extern "C"
void kernel_main(multiboot_info_t * mb_info, uint32_t stack_size, uintptr_t esp) {
	interrupts_disable();

	init_fbe(mb_info);

	terminal_initialize(); 

	char str_mem_buff[50] = {0};
	sprintf(str_mem_buff, "total memory:  %d kb\n",mb_info->mem_upper + mb_info->mem_lower);
	terminal_writestring((char*)str_mem_buff);

	// char str_mb_buff[50] = {0};
	// sprintf(str_mb_buff, "memory ptr:  %x \n",mb_info->mem_lower);
	// terminal_writestring((char*)str_mb_buff);

	char str_bb_buff[50] = {0};
	sprintf(str_bb_buff, "vga:  %dx%d pitch: %d \n",frame_width, frame_height, frame_pitch);
	terminal_writestring((char*)str_bb_buff);

	// char str_fb_buff[50] = {0};
	// sprintf(str_fb_buff, "frontbuffer ptr:  %x \n",fb_loc);
	// terminal_writestring((char*)str_fb_buff);



	//setpx(100,100,RGBA(0x00FF00));
	init_screens();

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

		// if (strncmp(buffer, "clear", 5) == 0) {
		// 	terminal_clear();
		// } 
		// else if (strncmp(buffer, "help", 4) == 0) {
		// 	string help = "List of commands\n"
		// 		"help:        prints a list of commands\n"
		// 		"clear:       clears the screen\n";

		// 	terminal_writestring((char*)help);
		// }
		// if (strncmp(buffer, "ls", 5) == 0) {
		// } 

	}
}