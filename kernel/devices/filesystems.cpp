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
        ATA::read(bus, drive, &headerBytes, 1, 0);

        uint8_t header[3] = {0xEB, 0x3C, 0x90}; // FAT16 header bytes

        for(int i = 0; i < 3; i++) {
            if(*(headerBytes + i*sizeof(char)) != header[i]) return false;
        }

        // the below variables scope out the "BIOS Parameter Block" (BPB)
        // TODO: make a struct that follows the byte pattern of the header so we can hydrate the class in with malloc+memset
        header_info.sectorSize = (*(headerBytes + 11*sizeof(char))) | (*(headerBytes + 12*sizeof(char)) << 8); // little-endian -> big-endian
        header_info.secPerCluster = *(headerBytes + 13*sizeof(char));
        header_info.numReservedSectors = *(headerBytes + 15*sizeof(char)); // offset 14 would be zero in like 99.9999% of cases
        header_info.numFATs = *(headerBytes + 16*sizeof(char));
        header_info.numDirectories = (*(headerBytes + 17*sizeof(char))) | (*(headerBytes + 18*sizeof(char)) << 8);
        header_info.numSectors = (*(headerBytes + 19*sizeof(char))) | (*(headerBytes + 20*sizeof(char)) << 8);
        header_info.numSectorsPerFAT = (*(headerBytes + 21*sizeof(char))) | (*(headerBytes + 22*sizeof(char)) << 8);

        // Below we have the Extended BIOSParameter Block (EBPB)
        volume_serial = (*(headerBytes + 39*sizeof(char))) 
                      | (*(headerBytes + 40*sizeof(char)) << 8) 
                      | (*(headerBytes + 41*sizeof(char)) << 16) 
                      | (*(headerBytes + 42*sizeof(char)) << 24);

        // terminal_printf("         Volume serial: %x\n", volume_serial);
        // terminal_printf("         Sector size: %x\n", header_info.sectorSize);

        return true;
    }

    void FAT16::readDirectoryTable(char* path) {
        //uint8_t * dir = 

        //fat_direntry_t* dir_table = new fat_direntry_t[16];

        terminal_printf("Browsing directory: %s\n", path);

        //ATA::resetATA(bus, drive);
        ATA::read(bus, drive, &dirBytes, 1, 512);

        terminal_writestring("\n");

        //memcpy(dir_table, dirBytes, sizeof(fat_direntry_t)*16);

        for(size_t i = 0; i < 4; i++) {
            for(size_t offset = 0; offset < 12; offset++) {
                char dirChar = *(dirBytes+sizeof(char)*offset+sizeof(fat_direntry_t)*i);

                if(dirChar != 0x20) terminal_putchar(dirChar, RGBA(0xFFFF00));
            }
            
            //terminal_printf("%x ", (int)(dirBytes[i] & 0xFF));
            //terminal_printf("%d\n", sizeof(fat_direntry_t));
            //terminal_writestring(dir_table[i].name);
            terminal_writestring("   DIR \n");
        }
    }
}