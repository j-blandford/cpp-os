#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include <gfx/vesa.h>

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

// TODO: turn this into a class which either creates, or uses an existing, surface object
//  this will allow us to have terminal windows :)


void terminal_writelog(char* data);
void terminal_writeerror(char* data);

void terminal_initialize(void);
void terminal_putchar(char c, RGBA color);
void terminal_clear(void);

void terminal_writestring(const char* data);
void terminal_writestring(const char* data, RGBA color);

void terminal_multistring(char* data1, RGBA color1, char* data2, RGBA color2);

void terminal_printf(const char* fmt, ...);

size_t tty_get_cursor_x();
void tty_set_cursor_x(size_t x);
void update_cursor(int row, int col);

int printf(const char* __restrict, ...);
int sprintf(char* buffer, const char* fmt, ...);
