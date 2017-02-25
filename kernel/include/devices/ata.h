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

#define ATA_CONTROL	0x206

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

    static void resetATA(int bus, int drive);
    static void wait(int bus, int drive, int mask, int waitForState);

    static std::vector<ATA_Device> findATA();

    // todo: ATAPI
    // todo: SCSI (maybe...)
};

void init_ata();