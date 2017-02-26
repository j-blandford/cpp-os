#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cpu/io.h>

#define ATA_BLOCKSIZE 512

#define ATA_PRIMARY     0
#define ATA_SECONDARY   1
#define ATA_MASTER_DRIVE    0
#define ATA_SLAVE_DRIVE     1

// ATA IRQs
#define ATA_IRQ0_M	32+14 // NEED TO REMAP THESE
#define ATA_IRQ0_S	32+15 // ^^^^
#define ATA_IRQ1_M	32+11 // ^^^^
#define ATA_IRQ1_S	32+9  // ^^^^

// ATA Flags when sending data
#define ATA_F_ECC	0x80
#define ATA_F_SLAVE	0x10        // slave drive
#define ATA_F_512_SEC	0x20	// 512byte sectors
#define ATA_F_LBA	0x40        // linear addressing

// ATA Status codes
#define ATA_STATUS_DRQ	0x08    // request data
#define ATA_STATUS_BSY	0x80  //busy

// In/output offsets
#define ATA_0_MASTER	0x1F0
#define ATA_0_SLAVE	    0x170
#define ATA_1_MASTER	0x1E8
#define ATA_1_SLAVE     0x168

// ATA registers
#define ATA_DATA	0   /* data register */
#define ATA_ERROR	1   /* error register */
#define ATA_COUNT	2   /* sectors to transfer */
#define ATA_SECTOR	3   /* sector number */
#define ATA_CYL_LO	4   /* low byte of cylinder number */
#define ATA_CYL_HI	5   /* high byte of cylinder number */
#define ATA_FDH		6   /* flags, drive and head */
#define ATA_STATUS	7
#define ATA_COMMAND	7

#define ATA_CTRL	0x206
#define ATA_CTRL_RST	0x04
#define ATA_CTRL_DISABLE_INT	0x02

#define ATA_COMMAND_IDLE		0x00
#define ATA_COMMAND_READ		0x20    /* read data */
#define ATA_COMMAND_WRITE		0x30    /* write data */
#define ATA_COMMAND_IDENTIFY		0xec

class ATA_Device {
public:
    char* name;
    uint16_t capacity;
};

class ATA {
public:
    // programmed i/o mode (all ATA devices support this)
    static uint16_t * readPIO(int bus, int drive, int size);
    static void writePIO(int bus, int drive, uint16_t * buffer, int size);

    static bool initializeDevice(int bus, int drive);

    static void resetATA(int bus, int drive);

    static bool prepare(int bus, int drive, int command, int num_blocks, int offset); // prepares the device for a command's data packet
    static bool read(int bus, int drive, uint8_t* buffer, int num_blocks, int offset);
    static bool wait(int bus, int drive, int mask, int waitForState);

    static std::vector<ATA_Device> findATA();

    // todo: ATAPI
    // todo: SCSI (maybe...)
};

void init_ata();