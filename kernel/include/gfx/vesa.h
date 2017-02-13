#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <string.h>

#include <cpu/multiboot.h>

#include <gfx/vga.h>

class RGBA {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        (*this).r = r;
        (*this).g = g;
        (*this).b = b;
        (*this).a = a;
    }

    RGBA(uint32_t hex) {
        (*this).r = hex >> 16;
        (*this).g = hex >> 8;
        (*this).b = hex;

        (*this).a = 0xFF; // this constructor only supports 32-bit colour, no transparency
    }

    ~RGBA() { }

    string toHex();

};

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

extern const RGBA color_transparency;
extern const RGBA color_black;
extern const RGBA color_red;
extern const RGBA color_green;
extern const RGBA color_blue;
extern const RGBA color_cyan;
extern const RGBA color_magenta;
extern const RGBA color_yellow;
extern const RGBA color_gray;
extern const RGBA color_light_gray;
extern const RGBA color_white;

// front-buffer, grabbed from Multiboot information
void init_fbe(multiboot_info_t * mb_info);

void update_buffer();

void putpx(unsigned int x, unsigned int y, RGBA color);
void setpx(unsigned int x, unsigned int y, RGBA col);

void       draw_circle(const uint32_t x, const uint32_t y,
                                      uint16_t radius, RGBA color);
void       fill_circle(const uint32_t x, const uint32_t y,
                                      uint16_t radius, RGBA color);

extern unsigned int frame_width, frame_height, frame_depth, frame_pitch;
extern uint8_t     *fb_loc;
extern uint8_t     *bb_loc; // back-buffer

// Font drawing and display functions
void drawchar_transparent(unsigned char c, int x, int y, RGBA fgcolor);