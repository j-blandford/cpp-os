global set_paging_bit
set_paging_bit:
    push ebp            ; set up the call frame
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000 ; set bit 31 of the CR0 register
    mov cr0, eax
    mov esp, ebp
    pop ebp             ; set up the return frame
    ret

global set_paging_pointer
set_paging_pointer:
    push ebp
    mov ebp, esp
    mov eax, [esp+8]    ; the first argument to the function
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret