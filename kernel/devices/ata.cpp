#include <stddef.h>
#include <stdint.h>

#include <std.h>
#include <tty.h>

#include <cpu/io.h>
#include <cpu/idt.h> 

#include <devices/ata.h>

const uint16_t device_offsets[2][2] = {
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

// volatile uint64_t timer_ticks;

// static void timer_handler(struct registers * registers)
// {
// 	timer_ticks++;

// }

// void timer_phase(int hz)
// {
// 	int divisor = 1193180 / hz;       /* Calculate our divisor */
// 	outportb(0x43, 0x36);             /* Set our command byte 0x36 */
// 	outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
// 	outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
// }



// void sleep(int ticks)
// {
// 	unsigned int eticks;
// 	eticks = timer_ticks + ticks;
// 	// while(timer_ticks < eticks)
// 	// {
// 	// 	__asm__ __volatile__ ("sti//hlt//cli");
// 	// }
// }

void ATA::resetATA(int bus, int drive) {
    // allows us to send more commands after the ATA_STATUS command
    // uint16_t base_offset = device_offsets[bus][drive];

    // outportb(base_offset + ATA_CONTROL, ATA_CONTROL_RESET);

    // outportb(ata_base[id] + ATA_CONTROL, 0);
}

void ATA::wait(int bus, int drive, int mask, int waitForState) {
    uint8_t state;

    uint16_t base_offset = device_offsets[bus][drive];

    // while(true) {
    //     state = inportb(base_offset + ATA_STATUS);

    //     if((state&mask) == waitForState) return 1;
    // }
}

std::vector<ATA_Device> ATA::findATA() {
    std::vector<ATA_Device> found_devices = std::vector<ATA_Device>();

    for(int bus = 0; bus < 2; bus++) {
        for(int drive = 0; drive < 2; drive++) {
            uint16_t base_offset = device_offsets[bus][drive];

            uint8_t connected = inportb(base_offset + ATA_STATUS);
            if(connected == 0xFF) {
                terminal_printf("[ATA] Device (%d,%d) not found\n", bus, drive);
                continue;
            } 

            terminal_printf("[ATA] FOUND DEVICE AT (%d,%d)!\n", bus, drive);

            // We have found an attached ATA device on this bus, let's grab some info on it :)
            ATA::resetATA(bus, drive);

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