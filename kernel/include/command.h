#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <std/string.h>
#include <std/vector.h>

#include <tty.h>

class Command {
public:
    static std::vector<string> split(char* command);
    static void Parse(char * buffer);
};