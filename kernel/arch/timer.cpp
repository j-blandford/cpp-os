#include <stddef.h>
#include <stdint.h>

#include <std.h>
#include <tty.h>

#include <cpu/io.h>
#include <cpu/idt.h> 

#include <arch/i386/timer.h>

volatile uint64_t timer_ticks;

static void timer_irq0(struct registers * registers) {
    timer_ticks++;
    return;
}

void PIT::setCount(int hz) {
    int count = 1193182 / hz;

    outportb(0x43, 0x34);  // ‭"0 011 01 00" on the command port

    outportb(0x40, count & 0xFF); // low byte
    outportb(0x40, (count >> 8) & 0xFF); // high byte
}

void PIT::sleep(int ticks) {
	uint64_t eticks = timer_ticks + ticks;

	while(timer_ticks < eticks)
	{
		__asm__ __volatile__ ("sti//hlt//cli");
	}
}

void PIT::initTimer() {
    timer_ticks = 0;
    PIT::setCount(1000);

    set_irq_handler(0, timer_irq0);

}

void PIT::testTimer() {
    for(int i = 0; i < 10; i++) {
        PIT::sleep(500);
        terminal_writestring("Waited 500ms.\n");
        update_buffer();
    }
}

void sleep(int ticks) {
    PIT::sleep(ticks);
}

void wait(int ticks) {
    PIT::sleep(ticks);
}