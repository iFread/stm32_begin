
// определение векторов

#include <stdint.h>

#include "vectors_stm32.h"


	//
void Default_Handler()
{
	asm volatile ("bkpt 0");
while(1)
{
	}
}
// определение векторов прерываний
//***************************



//**************************
typedef void (*pHandler)();
extern uint32_t * _estack;

//struct
//{

//	} vec_tabvles;
__attribute__((section (".isr_vector"),used))
 pHandler isr_vector[]=
{
		(pHandler) &_estack,
		Reset_Handler,
		NMI_Handler,
		HardFault_Handler,
		MemMangeFault,
		BusFault,
		UsageFault,
		0,0,0,0,   // Reserved
		SVC_Handler,
        Debug_Handler,
		0,
		PendSV_Handler,
		SysTick_Handler,
// -------------------------------
// External Interrupt
		WWDG_IRQHandler,
		PVD_Yandler,
		Tamper_Handler,
		RTC_Handler,
		 FLASH_Handler,
		 RCC_Handler,
		 EXTI0_Handler,
		 EXTI1_Handler,
		 EXTI2_Handler,
		 EXTI3_Handler,
		 EXTI4_Handler,
		 DMA1_Ch1_Handler,
		 DMA1_Ch2_Handler,
		 DMA1_Ch3_Handler,
		 DMA1_Ch4_Handler,
		 DMA1_Ch5_Handler,
		 DMA1_Ch6_Handler,
		 DMA1_Ch7_Handler,
		 ADC1_2_Handler,
		 USB_HP_CAN_TX_Handler,
		 USB_LP_CAN_RX0_Handler,
		 CAN_RX1_Handler,
		 CAN_SCE_Handler,
		 EXTI9_5_Handler,
		 TIM1_BRK_Handler,
		 TIM1_UP_Handler,
		 TIM1_TRG_COM_Handler,
		 TIM1_CC_Handler,
		 TIM2_Handler,
		 TIM3_Handler,
		 0,
		 I2C_EV_Handler,
		 I2C_Er_Handler,
		 0,
		 0,
		 SPI1_Handler,
		 0,
		 USART1_Handler,
		 USART2_Handler,
         0,
		 EXTI_15_10_Handler,
		 RTC_ALARM_Handler,
		 USBWakeUp_Handler

		};


 extern void __start_();
 //extern void _exit(int);
void Reset_Handler()
{
__start_();
}

void __attribute__ ((section(".after_vectors"),weak,used))
NMI_Handler()
{
	asm volatile ("bkpt 0");
		while (1)
		    {
		    }

}
void __attribute__ ((section(".after_vectors"),weak,used))
HardFault_Handler()
{
	//asm volatile ("bkpt 0");
	struct
	  {
	    uint32_t r0;
	    uint32_t r1;
	    uint32_t r2;
	    uint32_t r3;
	    uint32_t r12;
	    uint32_t lr;
	    uint32_t pc;
	    uint32_t psr;
	  }*stack_ptr; //Указатель на текущее значение стека(SP)


	  asm(
	      "TST lr, #4 \n" //Тестируем 3ий бит указателя стека(побитовое И)
	      "ITE EQ \n"   //Значение указателя стека имеет бит 3?
	      "MRSEQ %[ptr], MSP  \n"  //Да, сохраняем основной указатель стека
	      "MRSNE %[ptr], PSP  \n"  //Нет, сохраняем указатель стека процесса
	      : [ptr] "=r" (stack_ptr)
	      );
	  asm("bkpt 0");
		while (1)
		    {
		    }

	}
void __attribute__ ((section(".after_vectors"),weak,used))
 MemMangeFault()
{
	asm volatile ("bkpt 0");
		while (1)
		    {
		    }
	}
void __attribute__ ((section(".after_vectors"),weak,used))
BusFault()
{
	asm volatile ("bkpt 0");
		while (1)
		    {
		    }

	}

void __attribute__ ((section(".after_vectors"),weak,used))
UsageFault()
{
	asm volatile ("bkpt 0");
		while (1)
		    {
		    }

}
void __attribute__((section(".after_vectors"),weak,used))
SVC_Handler()
{
	asm volatile ("bkpt 0");
		while (1)
		    {
		    }

}
void __attribute__((section(".after_vectors"),weak,used))
Debug_Handler()
{
	asm volatile ("bkpt 0");
		while (1)
		    {
		    }
	}

void __attribute__((section(".after_vectors"),weak,used))
PendSV_Handler()
{
	asm volatile ("bkpt 0");
	while (1)
	    {
	    }

}
void __attribute__(( weak,used))
SysTick_Handler()
{
	asm volatile ("bkpt 0");

	}


