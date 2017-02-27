#pragma once

#include <stddef.h>

namespace Filesystems {

    // TODO: Create base "FS" interface that can be inherited, providing certain functions which are independent of the drive
    typedef struct fat_header {
        unsigned short sectorSize;     // sector size, usually 512
        int secPerCluster;      // how many sectors make up a cluster (usually 2)

        int numReservedSectors; // number of pre-data reserved sectors (usually 2)

        int numFATs;            // number of file allocation tables (usually 2)
        uint16_t numSectorsPerFAT; // number of sectors in each file allocation table

        uint16_t numDirectories;
        uint32_t numSectors;    // total number of sectors in the volume.
    }__attribute__((packed)) fat_header_t;

    typedef struct fat_direntry {
        char name[11];
        char attributes;
        char reserved;

        char crt_tenths;     // creation time (10ths seconds)
        short crt_time;      // creation time (H:m:s)
        short crt_date;      // creation date
        short lst_date;      // last access date

        short cluster_high;  // high byte of cluster number (zero for FAT16)
        
        short mod_time;      // modification time (H:m:s)
        short mod_date;      // modification date

        short cluster_id;  // low byte of cluster number (really important)
        
        int file_size;
    }__attribute__((packed)) fat_direntry_t;

    class FAT16 {
        uint8_t* headerBytes; 
        fat_header_t header_info;

    public:
        uint16_t port;
        uint32_t volume_serial;
        int bus;
        int drive;

        uint8_t* dirBytes; 

        FAT16() { }
        FAT16(const uint16_t& port, const int& bus, const int& drive) : port(port), bus(bus), drive(drive) { }
        
        ~FAT16() { }

        void setPort(uint16_t ioPort);

        bool parseHeader();
        void readDirectoryTable(char* path = "/");
    };
}