#include <stdbool.h>
#include <stdint.h>

#include <memory.h>

#include <cpu/multiboot.h>

#include <gfx/vga.h>
#include <gfx/vesa.h>
#include <gfx/terminus.h>
unsigned int frame_width, frame_height, frame_depth, frame_pitch;
uint8_t     *fb_loc;
uint8_t     *bb_loc; // back-buffer

void putpx(unsigned int x, unsigned int y, uint32_t color)
{
    RGBA col(color);

    if(x>=frame_width || y>=frame_height)
        return;
    unsigned where = x*(frame_depth/8) + y*frame_pitch;
    bb_loc[where + 0] = col.b;
    bb_loc[where + 1] = col.g;
    bb_loc[where + 2] = col.r;
    
}

void setpx(unsigned int x, unsigned int y, RGBA col)
{
    if(x>=frame_width || y>=frame_height)
        return;
        
    unsigned where = x*(frame_depth/8) + y*frame_pitch;
    bb_loc[where + 0] = col.b;
    bb_loc[where + 1] = col.g;
    bb_loc[where + 2] = col.r;
}

void fill_circle(const uint32_t x, const uint32_t y, uint16_t radius, RGBA color)
{
    while(radius > 0)
    {
        unsigned xm=0;
        int delta=1-2*radius, error=0, ym=radius;
        while(1)
        {
            setpx(x+xm, y+ym, color);
            setpx(x-xm, y+ym, color);
            setpx(x+xm, y-ym, color);
            setpx(x-xm, y-ym, color);
            setpx(x+xm-1, y+ym, color);
            setpx(x-xm+1, y+ym, color);
            setpx(x+xm-1, y-ym, color);
            setpx(x-xm+1, y-ym, color);
            error = 2 * (delta + ym) - 1;
            if ((delta < 0) && (error <= 0) && ym != 0)
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
            if(ym == 0) break;
            ym--;
        }
        radius--;
    }
}

void init_fbe(multiboot_info_t * mb_info) {
    fb_loc = (uint8_t*)(int)mb_info->framebuffer_addr;

    frame_width = mb_info->framebuffer_width;
    frame_height = mb_info->framebuffer_height;
    frame_depth = mb_info->framebuffer_bpp;

    frame_pitch = mb_info->framebuffer_pitch;

    bb_loc = (uint8_t*)(int)malloc(frame_height*frame_pitch);
}



void drawchar_transparent(unsigned char c, int x, int y, RGBA fgcolor) {
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};

	uint8_t * glyph = font_terminus[c];
 
	for(cy=0;cy<16;cy++){
		for(cx=0;cx<8;cx++){
			if(glyph[cy]&mask[7-cx]) 
                setpx(x+cx,y+cy-12, fgcolor);
		}
	}
}

void update_buffer() {
    // Copy back buffer to front buffer in one big chunk
   memcpy((uint8_t *)fb_loc, (uint8_t *)bb_loc, frame_height*frame_pitch);
}