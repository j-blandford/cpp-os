#pragma once

#include <stddef.h>

namespace Filesystems {

    // TODO: Create base "FS" interface that can be inherited, providing certain functions which are independent of the drive
    typedef struct fat_header {
        unsigned short sectorSize;     // sector size, usually 512
        int secPerCluster;      // how many sectors make up a cluster (usually 2)

        int numFATs;            // number of file allocation tables (usually 2)
        uint16_t numSectorsPerFAT; // number of sectors in each file allocation table

        uint16_t numDirectories;
        uint32_t numSectors;    // total number of sectors in the volume.
    }__attribute__((packed)) fat_header_t;

    class FAT16 {
        uint8_t* headerBytes; 
        fat_header_t header_info;

    public:
        uint16_t port;
        int bus;
        int drive;

        FAT16() { }
        FAT16(const uint16_t& port, const int& bus, const int& drive) : port(port), bus(bus), drive(drive) { }
        
        ~FAT16() { }

        void setPort(uint16_t ioPort);
        bool parseHeader();
    };
}