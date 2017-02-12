#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>
#include <tty.h>

#include <cpu/io.h>

#include <gfx/vga.h>
#include <gfx/vesa.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static int X_FONTWIDTH = 8;
static int Y_FONTWIDTH = 12; 
static vec2 cursor_pos = { 0, 0 };

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	terminal_writelog("TTY Initialised");

    string init_string = "Terminal Initialised"; 
    init_string += string(" - C++ functions enabled! \n"); 

    terminal_writestring((char * )init_string); 
}

void terminal_writelog(const char* data) {
	terminal_writestring("[ ");
	terminal_writestring_color("LOG", VGA_COLOR_GREEN);
	terminal_writestring(" ] ");
	terminal_writestring(data);
	terminal_writestring("\n");
}

void terminal_writeerror(const char* data) {
	terminal_writestring("[ ");
	terminal_writestring_color("ERR", VGA_COLOR_LIGHT_RED);
	terminal_writestring(" ] ");
	terminal_writestring(data);
	terminal_writestring("\n");
}

void terminal_scrollup(void) {
	terminal_row = VGA_HEIGHT-1;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = terminal_buffer[index+VGA_WIDTH];//vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, color_rgba color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;

	terminal_buffer[index] = vga_entry(c, VGA_COLOR_WHITE);

	drawchar_transparent(c, x*X_FONTWIDTH, y*Y_FONTWIDTH, color);
}

void terminal_clear(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

	for(size_t x = 0; x < frame_width; x++) {
		for(size_t y = 0; y < frame_height; y++) {
			setpx(x, y, color_black);
		}
	}
}


void terminal_putchar(char c, color_rgba color) {
	unsigned char uc = c;

	if(c == '\n') {
    	terminal_column = 0;
   		terminal_row++;
    	return;
    } 

	terminal_putentryat(uc, color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}

	update_cursor(terminal_row, terminal_column);
//	update_buffer();
}

void terminal_write(const char* data, size_t size) {

	if(terminal_row == VGA_HEIGHT) {
		terminal_scrollup();
	}

	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i], color_white);
	}
}

void terminal_write_color(const char* data, size_t size, uint8_t color) {

	if(terminal_row == VGA_HEIGHT) {
		terminal_scrollup();
	}

	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i], color_white);
	}
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen((char * )data));
}

void terminal_writestring_color(const char* data, uint8_t color) {
	terminal_write_color(data, strlen((char * )data), color);
}

size_t tty_get_cursor_x() {
	return terminal_column;
}

void tty_set_cursor_x(size_t x) {
	terminal_column = x;

	update_cursor(terminal_row, x);
}

void update_cursor(int row, int col)
{
	unsigned short position=(row*80) + col;

	terminal_putentryat('|', color_black, cursor_pos.x, cursor_pos.y); // remove old caret

	terminal_putentryat('|', color_green, col+1, row);

	cursor_pos.x = col+1;
	cursor_pos.y = row;

	// cursor LOW port to vga INDEX register
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, (unsigned char)(position&0xFF));
	
	// cursor HIGH port to vga INDEX register
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (unsigned char )((position>>8)&0xFF));

}