/* multiboot.h - the header for Multiboot */
/* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option)any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* Macros. */
#pragma once

/* The magic number for the Multiboot header. */
#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

/* The flags for the Multiboot header. */
#ifdef __ELF__
# define MULTIBOOT_HEADER_FLAGS         0x00000003
#else
# define MULTIBOOT_HEADER_FLAGS         0x00010003
#endif

/* The magic number passed by a Multiboot-compliant boot loader. */
#define MULTIBOOT_BOOTLOADER_MAGIC      0x2BADB002

/* The size of our stack (16KB). */
#define STACK_SIZE                      0x4000

/* C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)_ ## sym
#else
# define EXT_C(sym)sym
#endif

#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

/* Do not include here in boot.S. */

/* Types. */

/* The Multiboot header. */
typedef struct multiboot_header {
    unsigned long magic; 
    unsigned long flags; 
    unsigned long checksum; 
    unsigned long header_addr; 
    unsigned long load_addr; 
    unsigned long load_end_addr; 
    unsigned long bss_end_addr; 
    unsigned long entry_addr; 
}multiboot_header_t; 

/* The symbol table for a.out. */
typedef struct aout_symbol_table {
    unsigned long tabsize; 
    unsigned long strsize; 
    unsigned long addr; 
    unsigned long reserved; 
}aout_symbol_table_t; 

/* The section header table for ELF. */
typedef struct elf_section_header_table {
    unsigned long num; 
    unsigned long size; 
    unsigned long addr; 
    unsigned long shndx; 
}elf_section_header_table_t; 

/* The Multiboot information. */
typedef struct multiboot_info {
    unsigned long flags; 
    unsigned long mem_lower; 
    unsigned long mem_upper; 
    unsigned long boot_device; 
    unsigned long cmdline; 
    unsigned long mods_count; 
    unsigned long mods_addr; 
    union {
        aout_symbol_table_t aout_sym; 
        elf_section_header_table_t elf_sec; 
    }u; 
    unsigned long mmap_length; 
    unsigned long mmap_addr; 
    uint32_t drives_length;
	uint32_t drives_addr;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;
    uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;
	union
	{
		struct
		{
			uint32_t framebuffer_palette_addr;
			uint16_t framebuffer_palette_num_colors;
		};
		struct
		{
			uint8_t framebuffer_red_field_position;
			uint8_t framebuffer_red_mask_size;
			uint8_t framebuffer_green_field_position;
			uint8_t framebuffer_green_mask_size;
			uint8_t framebuffer_blue_field_position;
			uint8_t framebuffer_blue_mask_size;
		};
	};
}multiboot_info_t; 

/* The module structure. */
typedef struct module {
    unsigned long mod_start; 
    unsigned long mod_end; 
    unsigned long string; 
    unsigned long reserved; 
}module_t; 

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
typedef struct memory_map {
    unsigned long size; 
    unsigned long base_addr_low; 
    unsigned long base_addr_high; 
    unsigned long length_low; 
    unsigned long length_high; 
    unsigned long type; 
}memory_map_t; 


#endif 