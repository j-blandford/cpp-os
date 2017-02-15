#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpu/multiboot.h>

#include <gfx/vga.h>
#include <gfx/vesa.h>

// This is my Surface class. It contains a small section of memory which can be blitted onto the back buffer. 
//       *** backbuffer ****
//   <--------  frame_width -->
//  |-------------------------| ^
//  |         Vec2 pos(x,y)   | |
//  |         |----------|    | |
//  |         | dim.x    |    |  frame_height
//  |         | dim.y    |    | |
//  |         |----------|    | |
//  |-------------------------| V

class Surface {
    uint8_t * buff_loc;
    int z_index;

public:
    uint32_t s_pitch;
    Vector2 pos;
    Vector2 dim;

    Surface() {
        
    }

    Surface(Vector2 pos, Vector2 dim) {
        (*this).pos = pos;
        (*this).dim = dim;

        (*this).z_index = 1; // 1 is just above the lowest terminal buffer

        float pitchRatio = (float)frame_pitch / (float)frame_width;

        (*this).s_pitch = pitchRatio*dim.x; 

        (*this).buff_loc = (uint8_t*)malloc(dim.y*(*this).s_pitch);
    }

    void apply(); // Apply the Surface to the backbuffer

    void bringToFront();
    void setZindex(int z_index);

    void setPixel(uint32_t x, uint32_t y, RGBA color); // Set a single pixel
    void setBackground(RGBA bg_color);
    void drawCircle(uint32_t x, uint32_t y, uint16_t radius, RGBA color);
};