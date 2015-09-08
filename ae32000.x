OUTPUT_FORMAT("elf32-ae32000-little", 
              "elf32-ae32000-big",
              "elf32-ae32000-little")

OUTPUT_ARCH(ae32000)

ENTRY(_Reset_Start)

GROUP(-lc -lgcc -lm -lstdc++ -lFileSystem -ljpeg)
SEARCH_DIR(.);

MEMORY
{
    ram : ORIGIN = 0xC0000000, LENGTH = 15M  
}

SECTIONS
{
  /* Read-only sections, merged into text segment: */
  .text      :
  { 
	*(.vects)
	*(.text .stub .text.* .gnu.linkonce.t.*)
	*(.gnu.warning)
    *(.rodata .rodata.*)
    *(.rodata1)
    *(.gnu.linkonce.r.*)
	*(.gcc_except_table)   
    ___ctors = . ;
    *(.ctors)
    ___ctors_end = . ;
    ___dtors = . ;
    *(.dtors)
    ___dtors_end = . ;
    _etext = . ;
    . = ALIGN(4);
    ___shadow_data = . ;
  } > ram

  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  PROVIDE (___shadow_data = . );
  PROVIDE (___dtors_end = . );

  .data :
  AT ( ADDR (.text) + SIZEOF(.text) )
  {
    . = ALIGN(4);
    _data_reload = . ;
    PROVIDE(___data_reload = . );
    *(.data .data.* .gnu.linkonce.d.*)
    *(.sdata .sdata.*)
    SORT(CONSTRUCTORS)
  } > ram
  _edata  =  .;
  PROVIDE (edata = .);

  .bss  : 
  {
    . = ALIGN(4);
   __bss_start = .;
   PROVIDE (__bss_start = . );
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*) 
   *(COMMON)
   __bss_end = .;
  } > ram

  _end = . ;
  PROVIDE (end = .);

  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line)  }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* These must appear regardless of  .  */

	._stack 0xC0FFFFF0 : 
	{ 
		_stack = .; 
		*(._stack) 
	}
}
PROVIDE(___data_size = SIZEOF(.data));
PROVIDE(___bss_size = SIZEOF(.bss));

