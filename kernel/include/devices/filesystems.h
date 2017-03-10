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

	// this is essentially a wrapper for linked list functionality in the allocation table
	class FileAllocationTable {
		int tableOffset;
	public:
		uint8_t table[512]; 
		FileAllocationTable(uint8_t* table_ptr, int offset) : tableOffset(offset){ memcpy(table, table_ptr, 512); }
		FileAllocationTable(int offset) : tableOffset(offset) { }
		FileAllocationTable() { }

		uint16_t nextSector(uint16_t index);
		std::vector<uint16_t> follow(uint16_t start); // returns a vector of 16-bit integers (the clusters in the chain)

		void printFAT();
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
		uint8_t headerBytes[512]; 
		uint8_t fatTable[512]; 

		fat_header_t header_info;

		FileAllocationTable fat;

	public:
		uint16_t port;
		uint32_t volume_serial;
		int bus;
		int drive;

		uint8_t* dirBytes; 

		FAT16() { 

		}
		FAT16(const uint16_t& port, const int& bus, const int& drive) : port(port), bus(bus), drive(drive) { }
		~FAT16() { }

		void printFAT();

		bool verifyDevice();

		uint16_t * getChain(std::vector<uint16_t> chain);

		std::vector<int> getSectorIndices(uint16_t startSector);

		bool parseHeader();
		bool getAllocationTable();
		std::vector<DirectoryEntry> readDirectoryTable(size_t sectorIndex);
		std::vector<DirectoryEntry> readDirectoryPath(string path);
	};
}