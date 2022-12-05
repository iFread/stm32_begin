#include <stdint.h>
#include <sys/types.h>

extern uint32_t _sdata;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sidata;

extern int main();
extern void __attribute__((section(".after_vectors"),noreturn, weak))
_start();
void __attribute__((section(".after_vectors"),noreturn))
 __start_();
extern void
__run_init_array();
extern void __run_fini_array();

void _init_data(uint32_t* from,uint32_t* begin,uint32_t* end);
void _init_bss(uint32_t* begin,uint32_t* end);
extern void (*__preinit_array_start[])()__attribute__((weak));
extern void (*__preinit_array_end[])()__attribute__((weak));
extern void (*__init_array_start[])()__attribute__((weak));
extern void (*__init_array_end[])()__attribute__((weak));
extern void (*__fini_array_start[])()__attribute__((weak));
extern void (*__fini_array_end[])()__attribute__((weak));

inline void
__attribute__((always_inline)) __run_init_array()
{
  int  count;
  int i;
count= __preinit_array_end-__preinit_array_start;
for(i=0;i<count;++i)
	 __preinit_array_start[i]();
//_init();
count= __init_array_end-__init_array_start;
for(i=0;i<count;++i)
	 __init_array_start[i]();

}
inline void
__attribute__((always_inline))
__run_fini_array (void)
{
  int count;
  int i;

  count = __fini_array_end - __fini_array_start;
  for (i = count; i > 0; i--)
    __fini_array_start[i - 1] ();

  // If you need to run the code in the .fini section, please use
  // the startup files, since this requires the code in crti.o and crtn.o
  // to add the function prologue/epilogue.
//  _fini(); // DO NOT ENABE THIS!
}
void _init_data(uint32_t* from,uint32_t* begin,uint32_t* end)
{

uint32_t *p=begin;
while(p<end)
	*p++=*from++;
   }

void _init_bss(uint32_t* begin,uint32_t* end)
{
	uint32_t *p=begin;
	while(p<end)
		*p++=0;


}
__attribute__((noreturn))
void _exit(int)
{
asm("nop");

}



void __start_()
{


//_start();
 _init_data(&_sidata,&_sdata,&_edata);
 _init_bss(&_sbss,&_ebss);
//asm ("nop");
 __run_init_array();
  main();
 __run_fini_array();

}


