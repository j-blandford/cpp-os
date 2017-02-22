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

      . = ALIGN(4096);

      .data BLOCK(4K) : ALIGN(4096)
      {
        data = .;
        *(.data)
      }

      . = ALIGN(4096);

      .bss BLOCK(4K) : ALIGN(4096)
      {
        bss = .;
        *(COMMON)
        *(.bss)
      }

      . = 64M;
      .kheap BLOCK(32M) : ALIGN(4096)
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