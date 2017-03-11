#include <stddef.h>
#include <stdint.h>

#include <std.h>

#include <arch/i386/timer.h>

#include <tty.h>

#include <cpu/io.h>
#include <cpu/idt.h> 

#include <devices/filesystems.h>
#include <devices/ata.h>

std::vector<Filesystems::FAT16*> found_devices = std::vector<Filesystems::FAT16*>();

const uint16_t device_offsets[2][2] = {
	{ ATA_0_MASTER, ATA_0_SLAVE },
	{ ATA_1_MASTER, ATA_1_SLAVE }
};

uint16_t * ATA::readPIO(int bus, int drive, int size) {
	uint16_t buffer[size/2] = {0};

	uint16_t buffer_byte[size] = {0};

	for(int i = 0; i < size; i++) {
		buffer_byte[i] = inports(device_offsets[bus][drive] + ATA_DATA);

	}

	if((int)(buffer_byte[27] >> 8) != 0)
		terminal_printf("[ATA] Device (%d,%d): ", bus, drive);

	for(int i = 27; i < 47; i++) {
		// uint16_t is actually TWO char's long!
		terminal_printf("%c", (int)(buffer_byte[i] >> 8));
		terminal_printf("%c", (int)(buffer_byte[i] & 0xFF));
	}

	terminal_writestring("\n");
	return buffer;
}

void ATA::writePIO(int bus, int drive, uint16_t * buffer, int size) {
	for(int i = size; i > 0; i-=2) {
		outports(device_offsets[bus][drive] + ATA_DATA, *buffer);
		*buffer++;
	}
}

void ATA::resetATA(int bus, int drive) {
	// allows us to send more commands after the ATA_STATUS command
	uint16_t base_offset = device_offsets[bus][drive];

	outportb(base_offset + ATA_CTRL, ATA_CTRL_RST);
	outportb(base_offset + ATA_CTRL, 0);
}

bool ATA::wait(int bus, int drive, int mask, int waitForState) {
	uint8_t state;

	uint16_t base_offset = device_offsets[bus][drive];

	uint64_t time = 0;

	while(true) {
		state = inportb(base_offset + ATA_STATUS);

		if((state&mask) == waitForState) { return true; }

		if(time > 10) { return false; }

		sleep(1);
		time++;
	}

	return false;
}

bool ATA::initializeDevice(int bus, int drive) {
	uint16_t base_offset = device_offsets[bus][drive];

	int flags = ATA_F_ECC | ATA_F_LBA | ATA_F_512_SEC;
	if(drive == 1) flags |= ATA_F_SLAVE;

	// send command ATA_COMMAND_IDENTIFY to the device to grab some of the information
	ATA::wait(bus, drive, ATA_STATUS_BSY, 0);

	outportb(base_offset + ATA_CTRL, 0);
	outportb(base_offset + ATA_COUNT, 0);
	outportb(base_offset + ATA_SECTOR, 0);
	outportb(base_offset + ATA_CYL_LO, 0);
	outportb(base_offset + ATA_CYL_HI, 0);
	outportb(base_offset + ATA_FDH, flags);
	outportb(base_offset + ATA_COMMAND, ATA_COMMAND_IDENTIFY);

	ATA::wait(bus, drive, ATA_STATUS_DRQ, ATA_STATUS_DRQ);

	uint16_t *buffer = ATA::readPIO(bus, drive, 512);

	return true;
}

bool ATA::prepare(int bus, int drive, int command, size_t num_blocks, int offset) {
	uint16_t base_offset = device_offsets[bus][drive];

	int flags = ATA_F_ECC | ATA_F_LBA | ATA_F_512_SEC;
	if(drive == 1) flags |= ATA_F_SLAVE;

	flags |= (offset >> 24) & 0xFF;

	ATA::wait(bus, drive,  ATA_STATUS_BSY | ATA_STATUS_RDY, ATA_STATUS_RDY);

	outportb(base_offset + ATA_CTRL, 0);
	outportb(base_offset + ATA_COUNT, num_blocks);
	outportb(base_offset + ATA_SECTOR, offset);
	outportb(base_offset + ATA_CYL_LO, (offset >> 8) & 0xFF);
	outportb(base_offset + ATA_CYL_HI, (offset >> 16) & 0xFF);
	outportb(base_offset + ATA_FDH, flags);
	outportb(base_offset + ATA_COMMAND, command);

	return true;  
}

bool ATA::read(int bus, int drive, uint16_t** buffer, size_t num_blocks, int offset) {
	uint16_t buffer_short[num_blocks * ATA_BLOCKSIZE] = {0};

	ATA::prepare(bus, drive, ATA_COMMAND_READ, num_blocks, offset);

	for(size_t block = 0; block < num_blocks; block++) {
		ATA::wait(bus, drive, ATA_STATUS_DRQ, ATA_STATUS_DRQ);

		uint16_t readValue;

		for(int i = 0; i < ATA_BLOCKSIZE; i++) {
			readValue = inports(device_offsets[bus][drive] + ATA_DATA);

			buffer_short[i+block*ATA_BLOCKSIZE] = readValue;
		}
	}

	*buffer = buffer_short;

	return true;
}


bool ATA::read(int bus, int drive, uint8_t** buffer, size_t num_blocks, int offset) {
	uint16_t buffer_short[num_blocks * ATA_BLOCKSIZE] = {0};
	uint8_t buffer_byte[num_blocks * ATA_BLOCKSIZE * 2] = {0};

	ATA::prepare(bus, drive, ATA_COMMAND_READ, num_blocks, offset);

	for(size_t block = 0; block < num_blocks; block++) {

		ATA::wait(bus, drive, ATA_STATUS_DRQ, ATA_STATUS_DRQ);

		uint16_t readValue;

		for(int i = 0; i < ATA_BLOCKSIZE; i++) {
			readValue = inports(device_offsets[bus][drive] + ATA_DATA);

			buffer_short[i+block*ATA_BLOCKSIZE] = readValue;
		}

		// Splits u16_t into 2*u8_t
		for(int i = 0; i < ATA_BLOCKSIZE; i++) {
			buffer_byte[i*2] = (int)(buffer_short[i]);
			buffer_byte[i*2+1] = (int)((buffer_short[i] >> 8) & 0xFF);
		}

		// for(int i = 0; i < 6; i++) {
		//     terminal_printf("%x ", (int)(buffer_short[i] & 0xFF));
		//     terminal_printf("%x ", (int)((buffer_short[i] >> 8) & 0xFF));
		// }

	}

	*buffer = buffer_byte;

	return true;
}

void ATA::findATA() {

	for(int bus = 0; bus < 2; bus++) {
		for(int drive = 0; drive < 2; drive++) {
			uint16_t base_offset = device_offsets[bus][drive];

			uint8_t connected = inportb(base_offset + ATA_STATUS);
			if(connected == 0xFF) {
				// this device isn't connected/configured
				continue;
			} 

			// We have found an attached ATA device on this bus, let's grab some info on it :)
			ATA::resetATA(bus, drive);
			ATA::initializeDevice(bus, drive);

			if(bus == 0 && drive == 0) {
				found_devices.push_back(new Filesystems::FAT16(base_offset, bus, drive));

				if(found_devices[0]->verifyDevice()) {
					terminal_multistring(">>>> Success! ", RGBA(0x00FF00), "Verified FAT16 drive\n", RGBA(0xFFFFFF));
				} 
				else {
					terminal_multistring(">>>> Error! ", RGBA(0xFF0000), " Drive is corrupted\n", RGBA(0xFFFFFF));
				}
			}
		}
	}
}

std::vector<Filesystems::DirectoryEntry> ATA::getDirectoryPath(int deviceIndex, char * path) {
	std::vector<Filesystems::DirectoryEntry> dir = std::vector<Filesystems::DirectoryEntry>();

	int folderDepth = 0;
	int pathIndex = 0;
	while(path[pathIndex] != '\0') {
		if(path[pathIndex] == '/') 
			folderDepth++;

		pathIndex++;
	}

	// this function splits "path" into tokens delimited by '/' and then extracts out each folder and 
	//		maps the structure (from the root node) to find the right table directory entry sector
	bool foundPath = false;
	bool error = false;
	size_t currentSector = 512; // 512 = root dir table sector

	if(strlen(path) > 2) {
		while(!foundPath && !error) {

			char* folderSplit = strtok(path, "/");
			int folderCount = 0;
			while(folderSplit != NULL) {		
				bool foundInner = false;

				std::vector<Filesystems::DirectoryEntry> currDir = found_devices[deviceIndex]->readDirectoryTable(currentSector);

				for(auto it = currDir.begin(); it != currDir.end(); it++) {
					if(strcmp((char*)(*it).name, folderSplit) == 0) {
						currentSector = found_devices[deviceIndex]->getSectorIndices((*it).location)[0];
						dir = found_devices[deviceIndex]->readDirectoryTable(currentSector);
						
						foundInner = true;
						break;
					}
				}

				if(foundInner) {
					currDir = dir;
					folderCount++;

					if(folderCount >= folderDepth) foundPath = true;
				} 
				else {
					// folder not found
					error = true;
				}

				folderSplit = strtok(NULL, "/");
			}
		}
	}
	else {
		// this means we just list the root directory
		dir = found_devices[deviceIndex]->readDirectoryTable(512);
		foundPath = true;
	}

	if(error || !foundPath) {
		terminal_printf("Error: directory not found\n");
	}
	

	return dir;
}

void init_ata() {
	//timer_phase(1000);
	//set_irq_handler(0, (isr_t)&timer_handler);

	terminal_writestring("------------------------------------\n");
	terminal_writestring("Identifying ATA devices...\n");
	ATA::findATA();
	terminal_writestring("------------------------------------\n");
}