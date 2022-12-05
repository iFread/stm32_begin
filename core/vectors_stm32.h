
#ifndef _VECTORS_ISR_H
#define _VECTORS_ISR_H



//void __start_();
extern void Reset_Handler();
extern void NMI_Handler();
extern void HardFault_Handler();
extern void MemMangeFault();
extern void BusFault();
extern void UsageFault();
extern void SVC_Handler();
extern void Debug_Handler();
extern void PendSV_Handler();
 void SysTick_Handler()__attribute__((weak));

void __attribute__((weak)) Default_Handler();


//External Interrupt handler
void __attribute__ ((weak, alias ("Default_Handler")))
   WWDG_IRQHandler();
void __attribute__((weak,alias("Default_Handler")))
   PVD_Yandler();
void __attribute__((weak,alias("Default_Handler")))
   Tamper_Handler();
void __attribute__((weak,alias("Default_Handler")))
  RTC_Handler();
void __attribute__((weak,alias("Default_Handler")))
  FLASH_Handler();
void __attribute__((weak,alias("Default_Handler")))
	RCC_Handler();
void __attribute__((weak,alias("Default_Handler")))
	EXTI0_Handler();
void __attribute__((weak,alias("Default_Handler")))
	EXTI1_Handler();
void __attribute__((weak,alias("Default_Handler")))
	EXTI2_Handler();
void __attribute__((weak,alias("Default_Handler")))
	EXTI3_Handler();
void __attribute__((weak,alias("Default_Handler")))
	EXTI4_Handler();
void __attribute__((weak,alias("Default_Handler")))
	DMA1_Ch1_Handler();
void __attribute__((weak,alias("Default_Handler")))
	DMA1_Ch2_Handler();
void __attribute__((weak,alias("Default_Handler")))
	DMA1_Ch3_Handler();
void __attribute__((weak,alias("Default_Handler")))
   DMA1_Ch4_Handler();
void __attribute__((weak,alias("Default_Handler")))
	DMA1_Ch5_Handler();
void __attribute__((weak,alias("Default_Handler")))
	DMA1_Ch6_Handler();
void __attribute__((weak,alias("Default_Handler")))
	DMA1_Ch7_Handler();
void __attribute__((weak,alias("Default_Handler")))
    ADC1_2_Handler();
void __attribute__((weak,alias("Default_Handler")))
 USB_HP_CAN_TX_Handler();
void __attribute__((weak,alias("Default_Handler")))
  USB_LP_CAN_RX0_Handler();
void __attribute__((weak,alias("Default_Handler")))
  CAN_RX1_Handler();
void __attribute__((weak,alias("Default_Handler")))
  CAN_SCE_Handler();
void __attribute__((weak,alias("Default_Handler")))
 EXTI9_5_Handler();
void __attribute__((weak,alias("Default_Handler")))
 TIM1_BRK_Handler();
void __attribute__((weak,alias("Default_Handler")))
 TIM1_UP_Handler();
void __attribute__((weak,alias("Default_Handler")))
 TIM1_TRG_COM_Handler();
void __attribute__((weak,alias("Default_Handler")))
 TIM1_CC_Handler();
void __attribute__((weak,alias("Default_Handler")))
 TIM2_Handler();
void __attribute__((weak,alias("Default_Handler")))
 TIM3_Handler();
 //0 or TIM4
void __attribute__((weak,alias("Default_Handler")))
  I2C_EV_Handler();
void __attribute__((weak,alias("Default_Handler")))
  I2C_Er_Handler();
//0
//0  I2C2_Handler
void __attribute__((weak,alias("Default_Handler")))
   SPI1_Handler();
// 0 -SPI2_Handler
void  __attribute__((weak,alias("Default_Handler")))
 USART1_Handler();

void __attribute__((weak,alias("Default_Handler")))
 USART2_Handler();

// 0 -USART3_Handler
void __attribute__((weak,alias("Default_Handler")))
    EXTI_15_10_Handler();
void __attribute__((weak,alias("Default_Handler")))
 RTC_ALARM_Handler();
void __attribute__((weak,alias("Default_Handler")))
 USBWakeUp_Handler();








#endif
