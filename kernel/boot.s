; boot.s - entry point
MB_HEADER_MAGIC equ 0x1BADB002              ;
MB_EAX_MAGIC equ 0x2BADB002                 ;
MB_PAGE_ALIGN	equ 1<<0                      ; align on 4kb boundary
MB_MEMORY_INFO	equ 1<<1                    ; request mem_info struct
MB_VIDEO_MODE equ 1<<2                      ; request a video mode
MB_HEADER_FLAGS equ (MB_PAGE_ALIGN | MB_MEMORY_INFO | MB_VIDEO_MODE)
MB_CHECKSUM equ -(MB_HEADER_MAGIC + MB_HEADER_FLAGS)

STACK_SIZE equ 32768

bits 32

extern code, bss, end, kernel_main

; put the multiboot header within the first 8kb of the kernel.
; this is achieved in the linker script
section ._multiboot_header
align 4
_multiboot_header:
            dd MB_HEADER_MAGIC          ;
            dd MB_HEADER_FLAGS          ;
            dd MB_CHECKSUM              ; defined as a simple mathematical function

            dd _multiboot_header        ; header address
            dd code                     ;
            dd bss                      ;
            dd end                      ;
            dd _boot                    ; entry point

            dd 0
            dd 1024                      ; preferred width
            dd 768                      ; preffered height
            db 24                       ; 32 bpp (8r/8g/8b/8a)
                
section .text
global _boot:function _boot.end-_boot
_boot:
            cli                       ; first order: disable interrupts

            finit                     ; fpu init
            mov esp, stack            ; setup stack

            push stack                ; pass stack start (param 3)
            push STACK_SIZE           ; and size to the kernel (param 2)
            push ebx                  ; and don't forget a pointer to the multiboot header (param 1)

            call kernel_main                ; int kmain (multiboot_t *multiboot_info, uint32_t stack_size, uintptr_t esp);

.halt:      cli
            hlt
            jmp .halt
.end:

%include "kernel/cpu/gdt.s"
%include "kernel/cpu/idt.s"
%include "kernel/cpu/interrupts.s"
%include "kernel/cpu/memory.s"

section .bss
align 16
  resb STACK_SIZE
stack: