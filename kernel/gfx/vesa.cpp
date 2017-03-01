#include <stdbool.h>
#include <stdint.h>

#include <memory.h>
#include <tty.h>

#include <cpu/multiboot.h>

#include <gfx/vga.h>
#include <gfx/surface.h>
#include <gfx/vesa.h>
#include <gfx/terminus.h>

unsigned int frame_width, frame_height, frame_depth, frame_pitch;
uint8_t     *fb_loc;
uint8_t     *bb_loc; // back-buffer

bool         *dirty_lines; // our "dirty" buffer

void init_screens() {

	screen_surfaces.push_back(Surface(Vector2(0,0), Vector2(frame_width,frame_height)));
	screen_surfaces[SURF_SCREEN].setBackground(RGBA(0x2a2b31));
	screen_surfaces[SURF_SCREEN].apply();

    //test_surfaces();

	// screen_surfaces.push_back(Surface(Vector2(250,250), Vector2(50,50)));
	// screen_surfaces[1].setBackground(RGBA(0xFFFFFF));
	// screen_surfaces[1].drawCircle(25, 25, 20, RGBA(0xFF0000));
	// screen_surfaces[1].apply();

}

void putpx(size_t x, size_t y, uint32_t color)
{
    RGBA col(color);

    if(x>=frame_width || y>=frame_height)
        return;
    unsigned where = x*(frame_depth/8) + y*frame_pitch;
    bb_loc[where + 0] = col.b;
    bb_loc[where + 1] = col.g;
    bb_loc[where + 2] = col.r;
    
}

void setpx(size_t x, size_t y, RGBA col)
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

    dirty_lines = (bool*)malloc(sizeof(bool)*frame_height);
    memset(dirty_lines, false, sizeof(bool)*frame_height);
}

void test_surfaces() {
    for(size_t i = 0; i < screen_surfaces.size(); i++) {
        terminal_printf("Surface %d -> pointer=%x\n", i, screen_surfaces[i].buff_loc );

        for(size_t row = 0; row < 10; row++) {
            uint32_t address = row*screen_surfaces[i].s_pitch;
            terminal_printf("(0,%d) : (%d %d %d)\n", row, screen_surfaces[i].buff_loc[address],
                screen_surfaces[i].buff_loc[address+1],
                screen_surfaces[i].buff_loc[address+2] );
        }
    }
}

void drawchar_transparent(unsigned char c, int x, int y, RGBA fgcolor) {
	size_t cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};

    uint32_t x_coord;
    uint32_t y_coord;

	uint8_t * glyph = font_terminus[c];
 
	for(cy=0;cy<16;cy++){
		for(cx=0;cx<8;cx++){
			if(glyph[cy]&mask[7-cx]) {
                x_coord = x+cx;
                y_coord = y+cy-12;

                screen_surfaces[SURF_SCREEN].setPixel(x_coord, y_coord, fgcolor);
                *(dirty_lines+sizeof(bool)*(y_coord)) = true; // update the dirty buffer for this y-value
            }
		}
	}

    // MOVE THIS SOMEWHERE BETTER 
    //screen_surfaces[SURF_SCREEN].apply();
}

void update_buffer(bool fullRefresh) {

    screen_surfaces[SURF_SCREEN].apply(false);

    // Copy back buffer to front buffer where the "dirty" buffer is 1
    // we are currently marking one line dirty and updating the line
    // this could be improved by creating little pixels of ~25x25 for the dirty buffer
    for(size_t y = 0; y < frame_height; y++ ) {
        if(fullRefresh || dirty_lines[y]) {
            memcpy(&(fb_loc[y*frame_pitch]), bb_loc + y*frame_pitch, frame_pitch);
        }
    }

    // reset the dirty buffer
    memset(dirty_lines, false, sizeof(bool)*frame_height);
}