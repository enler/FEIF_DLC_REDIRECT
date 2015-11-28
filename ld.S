OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(_start)

SECTIONS
{
  . = 0x610168;
  .text       : { *(.text) }
  .rodata     : { *(.rodata) }
  .data       : { *(.data) }
  . = 0x780038;
  .bss        : { *(.bss) }
}