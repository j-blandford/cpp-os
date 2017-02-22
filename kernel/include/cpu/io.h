#pragma once

#include <stddef.h>
#include <stdint.h>

#define interrupts_disable() __asm__ __volatile__ ("cli")
#define interrupts_enable() __asm__ __volatile__ ("sti")


uint8_t inportb (uint16_t port);
void outportb (uint16_t port, uint8_t data);

uint16_t inports (uint16_t port);
void outports (uint16_t port, uint16_t data);

uint32_t inportl (uint16_t port);
void outportl (uint16_t port, uint32_t data);

__attribute__((noreturn)) void halt();

 //outputs a character to the debug console
 #define BochsConsolePrintChar(c) outportb(0xe9, c)
 //stops simulation and breaks into the debug console
 #define BochsBreak() outportl(0x8A00,0x8A00); outportl(0x8A00,0x08AE0);
 
void BochsConsolePrint(char *s);
