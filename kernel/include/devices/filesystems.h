#pragma once

#include <stddef.h>

namespace Filesystems {

	//static std::vector<string> tokenizePath(string path);

	enum FATAttributes : uint8_t {
		noEntry = 0x00,
		shortNameFile = 0x20,
		shortName = 0x10,
		longName = 0x0F
	};

	enum FATPermissions {
		READ_ONLY = 1 << 0,
		READ_WRITE = 1 << 1
	};

	class DirectoryEntry {
	public:
		char * name;
		char*  altName;     // used in FAT16 for the long-hand name

		FATAttributes attr;

		// TODO: Create a Timestamp class
		short mod_time;      // modification time (H:m:s)
		short mod_date;      // modification date
		short crt_time;      // creation time (H:m:s)
		short crt_date;      // creation date

		FATPermissions permissions;
		short location;     // generic "cluster" location

		DirectoryEntry() : name(new char[50]), permissions(FATPermissions::READ_WRITE), location(0)  { }
		DirectoryEntry(char* name) : permissions(FATPermissions::READ_WRITE), location(0) { }
		~DirectoryEntry() {
			delete name;
			delete altName;
		}
	};

	// TODO: Flesh out this stub class
	class FS_Device {
	public:
		virtual std::vector<DirectoryEntry> readDirectoryTable(size_t sectorIndex) = 0;
		char* name;
		uint16_t capacity;
	};

	typedef struct fat_header {
		unsigned short sectorSize;     // sector size, usually 512
		int secPerCluster;      // how many sectors make up a cluster (usually 2)

		int numReservedSectors; // number of pre-data reserved sectors (usually 2)

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
		uint32_t volume_serial;
		int bus;
		int drive;

		uint8_t* dirBytes; 

		FAT16() { }
		FAT16(const uint16_t& port, const int& bus, const int& drive) : port(port), bus(bus), drive(drive) { }
		~FAT16() { }

		void setPort(uint16_t ioPort);

		bool parseHeader();
		std::vector<DirectoryEntry> readDirectoryTable(size_t sectorIndex);
	};
}