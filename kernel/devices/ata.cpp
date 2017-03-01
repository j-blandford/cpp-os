#include <stddef.h>
#include <stdint.h>

#include <std.h>

#include <arch/i386/timer.h>

#include <tty.h>

#include <cpu/io.h>
#include <cpu/idt.h> 

#include <devices/filesystems.h>
#include <devices/ata.h>

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

        PIT::sleep(1);
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
	outportb(base_offset + ATA_SECTOR, offset & 0xFF);
	outportb(base_offset + ATA_CYL_LO, (offset >> 8) & 0xFF);
	outportb(base_offset + ATA_CYL_HI, (offset >> 16) & 0xFF);
	outportb(base_offset + ATA_FDH, flags);
	outportb(base_offset + ATA_COMMAND, command);

    return true;  
}

bool ATA::read(int bus, int drive, uint8_t* buffer, size_t num_blocks, int offset) {
    
    uint16_t buffer_byte[num_blocks * ATA_BLOCKSIZE] = {0};

    ATA::prepare(bus, drive, ATA_COMMAND_READ, num_blocks, offset);

    for(size_t block = 0; block < num_blocks; block++) {

        ATA::wait(bus, drive, ATA_STATUS_DRQ, ATA_STATUS_DRQ);

        for(int i = 0; i < ATA_BLOCKSIZE; i++) {
            uint16_t readValue = inports(device_offsets[bus][drive] + ATA_DATA);
            buffer_byte[i+block*ATA_BLOCKSIZE] = readValue;

            buffer[i*2 + block*ATA_BLOCKSIZE] = readValue & 0xFF;
            buffer[i*2+1 + block*ATA_BLOCKSIZE] = (readValue >> 8) & 0xFF;
        }

        for(int i = 0; i < ATA_BLOCKSIZE; i++) {
            // uint16_t is actually TWO char's long!
            //    terminal_printf("%x ", (int)(buffer_byte[i] & 0xFF));
            //   terminal_printf("%x ", (int)((buffer_byte[i] >> 8) & 0xFF));

        }

        // terminal_printf("%x ", (int)(buffer_byte[0] & 0xFF));
        // terminal_printf("%x ", (int)((buffer_byte[0] >> 8) & 0xFF));

        //terminal_printf("%x ", buffer[0]);
        //terminal_printf("%x ", buffer[1]);

    }

    return true;
}

std::vector<ATA_Device> ATA::findATA() {
    std::vector<ATA_Device> found_devices = std::vector<ATA_Device>();

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
                Filesystems::FAT16 device = Filesystems::FAT16(base_offset, bus, drive);

                if(device.parseHeader()) {
                    terminal_multistring(">>>> Success! ", RGBA(0x00FF00), "Verified FAT16 drive\n", RGBA(0xFFFFFF));

                    device.readDirectoryTable("/");
                } else {
                    terminal_multistring(">>>> Error! ", RGBA(0xFF0000), " Drive is corrupted\n", RGBA(0xFFFFFF));
                }                
            }
        }
    }
    return found_devices;
}

void init_ata() {
    //timer_phase(1000);
	//set_irq_handler(0, (isr_t)&timer_handler);

    terminal_writestring("\n------------------------------------\n");
    terminal_writestring("Identifying ATA devices...\n");
    std::vector<ATA_Device> ata_devices = ATA::findATA();
    terminal_writestring("\n------------------------------------\n");
}