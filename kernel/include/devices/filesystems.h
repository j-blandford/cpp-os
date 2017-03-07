#pragma once

#include <stddef.h>

#include <devices/ata.h>

#define PERM_READ 1U << 1
#define PERM_WRITE 1U << 2

namespace Filesystems {

	static std::vector<string> tokenizePath(string path);

	class DirectoryEntry {
	public:
		char * name;
		char*  altName;     // used in FAT16 for the long-hand name

		// TODO: Create a Timestamp class
		short mod_time;      // modification time (H:m:s)
		short mod_date;      // modification date
		short crt_time;      // creation time (H:m:s)
		short crt_date;      // creation date

		unsigned long permissions;
		short location;     // generic "cluster" location

		DirectoryEntry() : permissions(PERM_READ | PERM_WRITE), location(0), name(new char[50]) { }
		DirectoryEntry(char* name) : permissions(PERM_READ | PERM_WRITE), location(0) { }
		~DirectoryEntry() {
			delete name;
			delete altName;
		}
	};

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

	class FAT16 : public ATA_Device {
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
		std::vector<DirectoryEntry> readDirectoryTable(char* path = "/");
	};
}