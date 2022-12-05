
/*__stack definition is required  by crt0 */
__stack = ORIGIN(RAM) + LENGTH(RAM) ;

_estack = __stack ; /*STM specific defenition */ 


__Main_Stack_Size = 1024 ;

PROVIDE(_Main_Stack_Size = __Main_Stack_Size) ;

__Main_Stack_Limit = __stack -  __Main_Stack_Size ;

PROVIDE(_Main_Stack_Limit = __Main_Stack_Limit) ;

_Minimum_Stack_Size = 256 ;


/*  Default Heap Defenition 
   Heap  start after the last statically allocated, and extends up to the stack
 */
 
 PROVIDE(_Heap_Begin = _end_noinit) ;
 PROVIDE(_Heap_Limit = __stack - __Main_Stack_Size) ;
 
 ENTRY(__start_) 

 
SECTIONS 
{

 . = ALIGN(4);

 .isr_vector ORIGIN(FLASH) : 
 {
  __vectors_start = ABSOLUTE(.); 
 __vectors_start_ = ABSOLUTE(.); 
 
   FILL(0xFF)
   KEEP(*(.isr_vector))
  
   KEEP(*(.cfmconfig))
  *(.after_vector .after_vector.*)
   
   } >FLASH
  
  /* *************************************
   here is .inits section */
   
    .inits :  ALIGN(4)
    {
        /* 
         * Memory regions initialisation arrays.
         *
         * Thee are two kinds of arrays for each RAM region, one for 
         * data and one for bss. Each is iterrated at startup and the   
         * region initialisation is performed.
         * 
         * The data array includes:
         * - from (LOADADDR())
         * - region_begin (ADDR())
         * - region_end (ADDR()+SIZEOF())
         *
         * The bss array includes:
         * - region_begin (ADDR())
         * - region_end (ADDR()+SIZEOF())
         *
         * WARNING: It is mandatory that the regions are word aligned, 
         * since the initialisation code works only on words.
         */
         
          __data_regions_array_start = .;
         LONG(LOADADDR(.data));  
         LONG(ADDR(.data));
        LONG(ADDR(.data)+SIZEOF(.data));
        
        LONG(LOADADDR(.data_CCMRAM));
        LONG(ADDR(.data_CCMRAM));
        LONG(ADDR(.data_CCMRAM)+SIZEOF(.data_CCMRAM));
        
        __data_regions_array_end = .;
        
        __bss_regions_array_start = .;
        
        LONG(ADDR(.bss));
        LONG(ADDR(.bss)+SIZEOF(.bss));
        
        LONG(ADDR(.bss_CCMRAM));
        LONG(ADDR(.bss_CCMRAM)+SIZEOF(.bss_CCMRAM));
        
        __bss_regions_array_end = .;
       
       

        /* End of memory regions initialisation arrays. */
    
        /*
         * These are the old initialisation sections, intended to contain
         * naked code, with the prologue/epilogue added by crti.o/crtn.o
         * when linking with startup files. The standalone startup code
         * currently does not run these, better use the init arrays below.
         */
		KEEP(*(.init))
		KEEP(*(.fini))

		. = ALIGN(4);

		/*
         * The preinit code, i.e. an array of pointers to initialisation 
         * functions to be performed before constructors.
         */
		PROVIDE_HIDDEN (__preinit_array_start = .);
        
        /*
         * Used to run the SystemInit() before anything else.
         */
		KEEP(*(.preinit_array_sysinit .preinit_array_sysinit.*))
        
        /* 
         * Used for other platform inits.
         */
		KEEP(*(.preinit_array_platform .preinit_array_platform.*))
        
        /*
         * The application inits. If you need to enforce some order in 
         * execution, create new sections, as before.
         */
		KEEP(*(.preinit_array .preinit_array.*))

		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);

		/*
         * The init code, i.e. an array of pointers to static constructors.
         */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		. = ALIGN(4);

		/*
         * The fini code, i.e. an array of pointers to static destructors.
         */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

    } >FLASH
   
   
   
 
   /* For some STMx device the beggining of startup code is stored in the .flashtext section*/
 
 
  .flashtext : ALIGN(4)
  {
   *(.flashtext .flashtext.*)
  } > FLASH
   
 
  .text : ALIGN(4)
  {
  *(.text .text.*)
   *(.rodata .rodata.* .constdata .constdata.*)
   *(vtable)	
   
    *(.glue_7)
        *(.glue_7t)
   
    } > FLASH
  
 
 
  /* ARM magic sections */
	.ARM.extab : ALIGN(4)
   	{
       *(.ARM.extab* .gnu.linkonce.armextab.*)
   	} > FLASH
   	
    . = ALIGN(4);
   	__exidx_start = .;   	
   	.ARM.exidx : ALIGN(4)
   	{
       *(.ARM.exidx* .gnu.linkonce.armexidx.*)
   	} > FLASH
   	__exidx_end = .;
   	
    . = ALIGN(4);
    _etext = .;
    __etext = .;
   .data_CCMRAM : ALIGN(4)
    {
       FILL(0xFF)
       *(.data.CCMRAM .data.CCMRAM.*)
       . = ALIGN(4) ;
    } > CCMRAM AT>FLASH

  _sidata = LOADADDR(.data) ;
  
  .data : ALIGN(4)
  {
    FILL(0xFF) 
  _sdata = . ;
  _data_start = . ;
  
  *(.data_begin .data_begin.*)
  
   *(.data .data.*)
   *(.data_end .data_end.*)
   . = ALIGN(4) ;
   
   _edata = . ;
   _data_end = . ;
    } >RAM AT>FLASH
   
   .bss_CCMRAM (NOLOAD) : ALIGN(4)
	{
		*(.bss.CCMRAM .bss.CCMRAM.*)
	} > CCMRAM
    
    .bss (NOLOAD) : ALIGN(4)  
    {
     _sbss = . ;
     __bss_start__ = . ;
     
     *(.bss_begin .bss_begin.*)
     *(.bss .bss.*)
     *(COMMON)
     
     *(.bss_end .bss_end.*)
     . = ALIGN(4) ;
     
     _ebss = . ;
     __bss_end__ = . ;
    
    } >RAM

 .noinit (NOLOAD) : ALIGN(4) 
 {
 _noinit = . ;
 *(.noinit .noinit.*)
 
 . = ALIGN(4) ;
 _end_noinit = . ;
 
 } >RAM


}
  