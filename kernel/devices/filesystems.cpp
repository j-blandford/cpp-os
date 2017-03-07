#include <stdbool.h> 
#include <stddef.h> 

#include <std/string.h>
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

	std::vector<DirectoryEntry> FAT16::readDirectoryTable(size_t sectorIndex) {
		std::vector<DirectoryEntry> directoryMap = std::vector<DirectoryEntry>();
		std::bitset<32> bitMask(0x5542AA8A); // this is used to select out the long filename's data

		// may be above 1 sector... look into this
		size_t nSectors = 1;

		// Grab the dir table
		ATA::read(bus, drive, &dirBytes, nSectors, sectorIndex);

		size_t index = 0;
		while(index < nSectors*ATA_BLOCKSIZE) {
			DirectoryEntry* dirEntry = new DirectoryEntry();

			dirEntry->attr = static_cast<FATAttributes>(dirBytes[index+11]);

			if(dirEntry->attr == FATAttributes::shortName || dirEntry->attr == FATAttributes::shortNameFile) {
				// Normal short name (8+3) structure
				char name[11] = {0};
				int length = 0;

				bool addSeperator = false; // adds a '.' char into the filename if 0x20 is found (might be bugged if the folder name has a space in it...)

				dirEntry->location = dirBytes[index+26] | (dirBytes[index+27] << 8); // 16-bit cluster location

				// the first 11 bytes store the full short foldername. Let us extract it out...
				for(size_t offset = 0; offset <= 11; offset++) {
					char dirChar = dirBytes[index+offset];

					if(dirChar != 0x20 && dirChar != 0x10) { // 0x20 and 0x10 are padding or "file+ext" seperator byte
						if(addSeperator) {
							dirEntry->name[length] = '.';
							length++;

							addSeperator = false;
						}

						dirEntry->name[length] = tolower(dirChar);
						length++;
					} 
					else {
						addSeperator = true;
					}
				}
			} 
			else if(dirEntry->attr == FATAttributes::longName) {
				// We have found a LONG FOLDERNAME
				char name[32];
				int length = 0;

				// Our foldername is embedded inside 32-bytes. We have to extract it out the long way
				for(size_t j = 0; j <= 32; j++) {
					// our bitmask contains a series of 0 and 1s which allow us to extract the name correctly
					if(bitMask.test(31-j) && dirBytes[index+j] != 0xFF && dirBytes[index+j] != 0x00) {
						dirEntry->name[length] = dirBytes[index+j];
						length++;
					}
				}
				index += 32; // skip the short-folder name representation at the moment
			}
			else if(dirEntry->attr == FATAttributes::noEntry) {
				// there isn't a file here, so just skip to the next index entry
				index += 32;
				continue;	
			}

			directoryMap.push_back(*dirEntry);

			index += 32;
		}

		return directoryMap;
	}
}