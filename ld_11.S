OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)

SECTIONS
{
  . = 0x6108E8;
  .text       : { *(.text) }
  .rodata     : { *(.rodata) }
  .data       : { *(.data) }
  . = 0x781038;
  .bss        : { *(.bss) }
}