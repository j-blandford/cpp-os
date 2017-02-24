global gdt_flush

; called as: gdt_flush(&gdt_ptr) in C
gdt_flush:        mov eax, [esp + 4]              ; retrive the 1st argument
                  lgdt [eax]                      ; load the gdt into the CPU

                  mov eax, 0x10                   ; 0x10 is the offset to the gdt data segment entry
                  mov ds, ax                      ; setup all segment registers to 0x10, we do not need them!
                  mov es, ax                      ; segmentation is NOT used anymore
                  mov fs, ax
                  mov gs, ax
                  mov ss, ax
                  jmp 0x08:.flush                 ; long jump to apply the new GDT
.flush:                                           ; simple long jump which returns from the function in C
                  ret