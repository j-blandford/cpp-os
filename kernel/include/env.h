#pragma once

#include <stddef.h>
#include <stdint.h>
#include <std/vector.h>
#include <std/string.h>

#include <cpu/io.h>

class pair {
public:
    char* key;
    string value;
    pair() { }
    pair(char* key, string value) : key(key), value(value) { }
};

class ENV {
public:
    static std::vector<pair> environment;

    static string get(char* envVar);
    static void set(string envVar, string envVal);
    
    static void setup();
};