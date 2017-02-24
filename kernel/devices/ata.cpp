#include <stddef.h>
#include <stdint.h>

#include <std.h>
#include <tty.h>
#include <cpu/io.h>

#include <devices/ata.h>

const int device_offsets[2][2] = {
    { ATA_0_MASTER, ATA_0_SLAVE },
    { ATA_1_MASTER, ATA_1_SLAVE }
};

uint16_t * ATA::readPIO(int bus, int drive, int size) {
    uint16_t* buffer = {0};

    for(int i = size; i > 0; i-=2) {
        *buffer = inports(device_offsets[bus][drive] + ATA_DATA);
        *buffer++;
    }

    return buffer;
}

void ATA::writePIO(int bus, int drive, uint16_t * buffer, int size) {
    for(int i = size; i > 0; i-=2) {
        outports(device_offsets[bus][drive] + ATA_DATA, *buffer);
        *buffer++;
    }
}

std::vector<ATA_Device> ATA::findATA() {
    std::vector<ATA_Device> found_devices = std::vector<ATA_Device>();
    for(int bus = 0; bus < 2; bus++) {
        for(int drive = 0; drive < 2; drive++) {
            
        }
    }
    return found_devices;
}

void init_ata() {
    terminal_writestring("\n------------------------------------\n");
    terminal_writestring("Identifying ATA devices...\n");
    std::vector<ATA_Device> ata_devices = ATA::findATA();
    terminal_writestring("\n------------------------------------\n");
}