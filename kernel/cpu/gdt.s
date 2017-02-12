global gdt_flush

gdt_flush:        mov eax, [esp + 4]              ; retrive the 1st argument
                  lgdt [eax]                      ; load the gdt

                  mov eax, 0x10                   ; 0x10 is the offset to the gdt data segment entry
                  mov ds, ax                      ; setup all segment registers
                  mov es, ax
                  mov fs, ax
                  mov gs, ax
                  mov ss, ax
                  jmp 0x08:.flush                 ; long jump to setup cs
.flush:
                  ret