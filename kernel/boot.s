; boot.s - entry point
MB_HEADER_MAGIC equ 0x1BADB002              ;
MB_EAX_MAGIC equ 0x2BADB002                 ;
MB_PAGE_ALIGN	equ 1<<0                      ; align on 4kb boundary
MB_MEMORY_INFO	equ 1<<1                    ; request mem_info struct
MB_VIDEO_MODE equ 1<<2                      ; request a video mode
MB_HEADER_FLAGS equ (MB_PAGE_ALIGN | MB_MEMORY_INFO | MB_VIDEO_MODE)
MB_CHECKSUM equ -(MB_HEADER_MAGIC + MB_HEADER_FLAGS)

STACK_SIZE equ 32768                        ; 32kb (gotta think big)

bits 32

extern code, bss, end, kernel_main

section ._multiboot_header

align 4
_multiboot_header:
                dd MB_HEADER_MAGIC          ;
                dd MB_HEADER_FLAGS          ;
                dd MB_CHECKSUM              ;

                dd _multiboot_header        ; header address
                dd code                     ;
                dd bss                      ;
                dd end                      ;
                dd _boot                    ; entry point

                dd 1                        ; Linear graphics mode
                dd 0                    ; No preferred width
                dd 0                      ; No preferred height
                db 0                       ; 32 bpp
                
section .text
global _boot:function _boot.end-_boot
_boot:
                  ; 9 tabs (2 spaces)       ; and 13 more to here
                  cli                       ; first order: disable interrupts
                  cmp eax, MB_EAX_MAGIC
                  jne .no_multiboot

                  finit                     ; fpu init

                  mov esp, stack            ; setup stack

                ;   push stack                ; pass stack start
                ;   push STACK_SIZE           ; and size to the kernel
                ;   push ebx                  ; and don't forget a pointer to the multiboot header

                  cmp eax, MB_EAX_MAGIC     ; magic is in the air

                  call kernel_main                ; int kmain (multiboot_t *multiboot_info, uint32_t stack_size, uintptr_t esp);

.halt:            cli                       ; if we ever return just go to sleep
                  hlt
                  jmp .halt

.no_multiboot:    ; NO (trying to do the least possible but still letting the user know = i'm lazy)
                  mov dword[0xb8000], 0x4f4e4f4f
                  jmp .halt
.end:

;%include "kernel/arch/i686/ports.s"
%include "kernel/cpu/gdt.s"
; %include "kernel/arch/i686/tss-flush.s"
;%include "kernel/arch/i386/idt-flush.s"
;%include "kernel/arch/i386/interrupts.s"
; %include "kernel/arch/i686/paging.s"

section .bss
align 16
  resb STACK_SIZE
stack: