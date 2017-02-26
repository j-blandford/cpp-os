#include <stdbool.h> 
#include <stddef.h> 

#include <tty.h> 
#include <cpu/idt.h> 

#include <devices/filesystems.h> 
#include <devices/ata.h> 

#include <gfx/vga.h>
#include <gfx/vesa.h>

#include <buffer.h> 

namespace Filesystems {
    void FAT16::setPort(uint16_t ioPort) {
        this->port = ioPort;
    }

    bool FAT16::parseHeader() {
        ATA::prepare(bus, drive, ATA_COMMAND_READ, 1, 0);
        ATA::read(bus, drive, headerBytes, 1, 0);

        uint8_t header[3] = {0xEB, 0x3C, 0x90}; // FAT16 header bytes

        for(int i = 0; i < 3; i++) {
            if(*(headerBytes + i*sizeof(char)) != header[i]) return false;
        }

        header_info.sectorSize = (*(headerBytes + 11*sizeof(char))) | (*(headerBytes + 12*sizeof(char)) << 8);

        terminal_printf("Sector size: %x\n", header_info.sectorSize);

        return true;
    }
}