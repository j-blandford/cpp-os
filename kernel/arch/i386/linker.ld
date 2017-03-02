ENTRY(start)
phys = 0x00100000; /* 1MiB kernel load */
SECTIONS
{
      /* put the multiboot header FIRST so it sits at < 8KiB for GRUB to locate it */
      ._multiboot_header : {
        *(._multiboot_header)
      }

      . = 1M;

      start = .;
      
      .text phys : AT(phys)
      {
        code = .;
        *(.text)
      }

      .rodata : ALIGN(4096)
      {
        *(.rodata)
      }

      .bss : ALIGN(4096)
      {
        bss = .;
        *(COMMON)
        *(.bss)
      }

      .data : ALIGN(4096)
      {
        *(.data)
      }

      end = .;
}