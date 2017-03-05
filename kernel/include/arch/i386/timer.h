#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cpu/io.h>

extern volatile uint64_t timer_ticks;

class PIT {
public:
    static void initTimer();
    static void testTimer();

    static void setCount(int hz);
    static void sleep(int ticks);
};

void sleep(int ticks);
void wait(int ticks);