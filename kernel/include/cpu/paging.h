#pragma once

#include <stddef.h>
#include <stdint.h>

// PageDirectory_t is a 32-bit (4 byte) struct
typedef struct PageDirectory_t {
    unsigned int present : 1;       // is the page present?
    unsigned int rw : 1;            // read-writable?
    unsigned int access_lvl : 1;    // ring 0 or ring 3?
    unsigned int wt_cache : 1;      // write-through cache
    unsigned int cache_disabled : 1;// more cache stuff (not going to use ATM)
    unsigned int accessed : 1;      // has the page been written to?
    unsigned int page_size : 1;     // set to 0 for 4 KiB pages
    unsigned int g_flag : 1;        // unused
    unsigned int free_bits : 4;     // some expandable bits we can use for something

    unsigned int address : 20;      // the meat of our struct :) address to the page table struct
}__attribute__((packed)) PageDirectory_t;

// PageTable_t is a 32-bit (4 byte) struct
typedef struct PageTable_t {
    unsigned int present : 1;       // is the page present?
    unsigned int rw : 1;            // read-writable?
    unsigned int access_lvl : 1;    // ring 0 or ring 3?
    unsigned int wt_cache : 1;      // write-through cache
    unsigned int cache_disabled : 1;// more cache stuff (not going to use ATM)
    unsigned int accessed : 1;      // has the page been written to?
    unsigned int dirty : 1;         
    unsigned int zero_flag : 1;         
    unsigned int g_flag : 1;        // unused
    unsigned int free_bits : 3;     // some expandable bits we can use for something

    unsigned int address : 20;      // the meat of our struct :)
}__attribute__((packed)) PageTable_t;

void paging_install();