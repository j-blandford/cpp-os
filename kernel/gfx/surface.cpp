#include <stdbool.h>
#include <stdint.h>

#include <cpu/multiboot.h>

#include <gfx/surface.h>
#include <gfx/vga.h>
#include <gfx/vesa.h>

void Surface::setPixel(uint32_t x, uint32_t y, RGBA color) {
    if(x>=this->dim.x || y>=this->dim.y)
        return;

    unsigned int where = x*(frame_depth/8) + y*this->s_pitch;
    
    this->buff_loc[where + 0] = color.b;
    this->buff_loc[where + 1] = color.g;
    this->buff_loc[where + 2] = color.r;
}

void Surface::drawCircle(uint32_t x, uint32_t y, uint16_t radius, RGBA color) {
    unsigned int xm=0;
    int delta=1-2*radius, error=0, ym=radius;

    while(1)
    {
        this->setPixel(x+xm, y+ym, color);
        this->setPixel(x-xm, y+ym, color);
        this->setPixel(x+xm, y-ym, color);
        this->setPixel(x-xm, y-ym, color);

        error = 2 * (delta + ym) - 1;
        if ((delta < 0) && (error <= 0) && ym !=0)
        {
            delta += 2 * ++xm + 1;
            continue;
        }

        error = 2 * (delta - xm) - 1;
        if ((delta > 0) && (error > 0) && ym != 0)
        {
            delta += 1 - 2 * --ym;
            continue;
        }
        xm++;

        delta += 2 * (xm - ym);
        if(ym == 0) return;
        ym--;
    }
}

void Surface::apply() {
    // Apply the surface to the back buffer.
    // ------------------------------------------------------------------**
    //     PLEASE NOTE: this function doesn't actually display the Surface! 
    //              --> You need to call "update_buffer()" to display it
    // ------------------------------------------------------------------**

    for(int y = 0; y < this->dim.y; y++) {
        // no SSE functions are currently implemented, so we have to copy it line by line
        int where = this->pos.x*(frame_depth/8) + (y+this->pos.y)*(frame_pitch); //

       // uint8_t* dest = bb_loc;
        memcpy(&bb_loc[where], &this->buff_loc[y*this->s_pitch], this->s_pitch);
    }
}

void Surface::bringToFront() {
    z_index = 10;
}
void Surface::setZindex(int z_index) {
    (*this).z_index = z_index;
}


void Surface::setBackground(RGBA bg_color) {
    for(int x = 0; x < this->dim.x; x++) {
        for(int y = 0; y < this->dim.y; y++) {
            this->setPixel(x, y, bg_color);
        }
    }
}