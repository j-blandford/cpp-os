#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include <gfx/vesa.h>

void terminal_writelog(const char* data);
void terminal_writeerror(const char* data);

void terminal_initialize(void);
void terminal_putchar(char c, RGBA color);
void terminal_clear(void);

void terminal_write(const char* data, size_t size);
void terminal_write_color(const char* data, size_t size, uint8_t color);

void terminal_writestring(const char* data);
void terminal_writestring_color(const char* data, uint8_t color);

size_t tty_get_cursor_x();
void tty_set_cursor_x(size_t x);
void update_cursor(int row, int col);