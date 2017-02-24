global idt_flush

; called as: idt_flush(&idt_ptr) in C
idt_flush:        mov eax, [esp + 4]        ; same as gdt_flush -> get first parameter from C
                  lidt [eax]                ; load the new idt
                  ret                       ; function return back to C execution