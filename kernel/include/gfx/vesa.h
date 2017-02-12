#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpu/multiboot.h>

#include <gfx/vga.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_rgba;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
} rect;

typedef struct {
    int x;
    int y;
} vec2;

extern const color_rgba color_transparency;
extern const color_rgba color_black;
extern const color_rgba color_red;
extern const color_rgba color_green;
extern const color_rgba color_blue;
extern const color_rgba color_cyan;
extern const color_rgba color_magenta;
extern const color_rgba color_yellow;
extern const color_rgba color_gray;
extern const color_rgba color_light_gray;
extern const color_rgba color_white;

// front-buffer, grabbed from Multiboot information
void init_fbe(multiboot_info_t * mb_info);

void update_buffer();

void putpx(unsigned int x, unsigned int y, uint32_t color);
void setpx(unsigned int x, unsigned int y, color_rgba col);

void       draw_circle(const uint32_t x, const uint32_t y,
                                      uint16_t radius, color_rgba color);
void       fill_circle(const uint32_t x, const uint32_t y,
                                      uint16_t radius, color_rgba color);

extern unsigned int frame_width, frame_height, frame_depth, frame_pitch;
extern uint8_t     *fb_loc;
extern uint8_t     *bb_loc; // back-buffer

// Font drawing and display functions
void drawchar_transparent(unsigned char c, int x, int y, color_rgba fgcolor);