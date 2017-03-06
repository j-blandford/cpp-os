#include <stdbool.h> 
#include <stddef.h> 

#include <std/bitset.h>

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

        std::bitset<32> bitMask(0x5542AA8A);
        // for(int i = 0; i < 32; i++ ) {
        //     if(i % 8 == 0 ) terminal_putchar('|', RGBA(0xFF0000));
        //     if(bitMask.test(i)) {
        //         terminal_putchar('1', RGBA(0xFFFF00));
        //     } else {
        //         terminal_putchar('0', RGBA(0xFFFF00));
        //     }
            
        //     //terminal_printf("%x ", bitMask[i]);
        //   //  terminal_putchar(bitMask[i], RGBA(0xFFFF00));
        // }

        ATA::read(bus, drive, &dirBytes, 1, 512);

        terminal_writestring("\n");

        // lets map this directory's structure...
        size_t index = 0;
        while(index < 512) {
            if(dirBytes[index+11] != 0x0F) {
                // Normal short folder name (8+3) structure
                for(size_t offset = 0; offset <= 11; offset++) {
                    char dirChar = dirBytes[index+offset];

                    if(dirChar != 0x20 && dirChar != 0x10) terminal_putchar(dirChar, RGBA(0xFFFF00));
                }
            } 
            else {
                // We have found a LONG FOLDERNAME
                for(int j = 0; j <= 32; j++) {
                    if(bitMask.test(31-j) && dirBytes[index+j] != 0xFF && dirBytes[index+j] != 0x00) 
                        terminal_putchar(dirBytes[index+j], RGBA(0xFFFF00));
                }

                index += 32; // skip the short-folder name representation at the moment
            }
            terminal_writestring("   DIR \n");

            index += 32;
        }

        // for(size_t i = 0; i < 4; i++) {
        //     for(size_t offset = 0; offset < 12; offset++) {
        //         char dirChar = *(dirBytes+sizeof(char)*offset+sizeof(fat_direntry_t)*i);

        //         if(dirChar != 0x20 && dirChar != 0x10) terminal_putchar(dirChar, RGBA(0xFFFF00));
        //     }
            
        //     //terminal_printf("%x ", (int)(dirBytes[i] & 0xFF));
        //     //terminal_printf("%d\n", sizeof(fat_direntry_t));
        //     //terminal_writestring(dir_table[i].name);
        //     terminal_writestring("   DIR \n");
        // }
    }
}