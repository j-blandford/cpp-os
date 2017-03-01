ENTRY(_start)
phys = 0x00100000;
SECTIONS
{
      ._multiboot_header : {
        *(._multiboot_header)
      }

      . = 1M;

      start = .; _start = .; __start = .;
      
      .text phys : AT(phys)
      {
        code = .;
        *(.text)
        *(.rodata)
      }

      . = 2M;

      .data : ALIGN(4096)
      {
        data = .;
        *(.data)
      }

      . = 32M;

      .bss : ALIGN(4096)
      {
        bss = .;
        *(COMMON)
        *(.bss)
      }

      . = 128M;
      .kheap : ALIGN(4096)
      {
        kernel_heap_start = .;
        *(.kheap)
      }

      end = .; _end = .; __end = .;

      /DISCARD/ :
      {
        *(.comment)
      }
}