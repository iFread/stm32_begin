#include <stdint.h>
//#include <core_ch3.h>
 #include "stm32f103x6.h"
#include <math.h>
#include "init.h"


void configure_sysclock()
{
	// настройка тактирования flash
	FLASH->ACR&=~(0x7<<FLASH_ACR_LATENCY_Pos);
	FLASH->ACR|=(0x2<<FLASH_ACR_LATENCY_Pos);

	// конфигурация тактирования мк : HSION, HSEON
//	1. HSION
	RCC->CR|=(1<<RCC_CR_HSION_Pos);
	while(!(RCC->CR & RCC_CR_HSIRDY))
	{

	}
	//2.HSEON
	RCC->CR|=(1<<RCC_CR_HSEON_Pos);
	while(!(RCC->CR & RCC_CR_HSERDY)){}

	// включить CSS контроль
	RCC->CR|=(1<<RCC_CR_CSSON_Pos);

	//RCC->CFGR  установка пределителей
	RCC->CFGR&=~(0x3<<RCC_CFGR_SW_Pos);
	RCC->CFGR|=(0x2<<RCC_CFGR_SW_Pos); // set pll
	while(!(RCC->CFGR& RCC_CFGR_SWS_Pos));

	RCC->CFGR&=~(0xf<<RCC_CFGR_HPRE_Pos); // ABH high-speed prescaller=0;

	// APB low speed prescaller =/2 etc 36 MHz
	RCC->CFGR&=~(0x7<<RCC_CFGR_PPRE1_Pos);
	RCC->CFGR|=(0x4<<RCC_CFGR_PPRE1_Pos);

	//APB-high-speed - not devided
	RCC->CFGR&=~(0x7<<RCC_CFGR_PPRE2_Pos);

	// выбор мютекса pll 0= hsi/2, 1= hse
	RCC->CFGR|=(1<<RCC_CFGR_PLLSRC_Pos);
	RCC->CFGR&=~(1<<RCC_CFGR_PLLXTPRE_Pos);

	// выбор умножителя pll: x9 для 72 MHz
RCC->CFGR&=~(0xF<<RCC_CFGR_PLLMULL_Pos);
RCC->CFGR|=(0x7<<RCC_CFGR_PLLMULL_Pos);

// set USB Prescaller
RCC->CFGR&=~(1<<RCC_CFGR_USBPRE_Pos);

RCC->CFGR&=~(0x3<<RCC_CFGR_MCO_Pos); // отключение внешнего тактирования

 // включение PLL после всех установок
RCC->CR|=(1<<RCC_CR_PLLON_Pos);
while(!(RCC->CR& RCC_CR_PLLRDY)){}

RCC->APB2ENR=((1<<RCC_APB2ENR_IOPCEN_Pos));
     GPIOC->CRH&=~(0xF<<GPIO_CRH_MODE13_Pos);
	 GPIOC->CRH|=(0x3<<GPIO_CRH_MODE13_Pos);

	 GPIOC->CRH&=~(0xf<<GPIO_CRH_MODE15_Pos);
	 GPIOC->CRH|=(0x3<<GPIO_CRH_MODE15_Pos);
//  GPIOC->CRH|=((1u<< 21u)|(1u<< 20u));
}

void Sys_Tick_config()
{
SysTick->CTRL|=((1<<SysTick_CTRL_CLKSOURCE_Pos)|(1<<SysTick_CTRL_TICKINT_Pos));
// далее установка бита Enable запустит таймер

}
void Sys_Tick_value(uint32_t val)
{
	SysTick->LOAD=val;
	Sys_Tick_start();
}

void Sys_Tick_start()
{
SysTick->CTRL|=(1<<SysTick_CTRL_ENABLE_Pos);
}


// включение таймера 2

 void Tim2_configure()
 {
	 // Тактирование
	 RCC->APB1ENR =(1<<RCC_APB1ENR_TIM2EN_Pos);
     NVIC_EnableIRQ(TIM2_IRQn);
      TIM2->PSC=7199;
      TIM2->ARR=10000;




	 TIM2->DIER|=(1<<TIM_DIER_UIE_Pos);
 }
 void Tim_start(void* v)
 {
	 ((TIM_TypeDef*)v)->CR1|=(1<<TIM_CR1_CEN_Pos);
			 ;
			 //->CR1|=(1<<TIM_CR1_CEN_Pos);
 }

 void Tim2_value(uint16_t val)
 {
     TIM2->ARR=val;

 }


void PWM_Tim2_config()
 {
	// Тактирование таймера и тактирование альтернативных функций
RCC->APB1ENR|=(1<<RCC_APB1ENR_TIM2EN_Pos);
RCC->APB2ENR|=(1<<RCC_APB2ENR_AFIOEN_Pos); // включение тактирования альтернативных функций


//Настройка таймера

// Запуск таймера в регистре CR1
TIM2->CR1|=(1<<TIM_CR1_CEN_Pos);

TIM2->CR1&=~(1<<TIM_CR1_UDIS_Pos|1<<TIM_CR1_URS_Pos|1<<TIM_CR1_OPM_Pos|1<<TIM_CR1_DIR_Pos);
TIM2->CR1&=~(0x3<<TIM_CR1_CMS_Pos);
//TIM2->CR1|=(1<<TIM_CR1_ARPE_Pos);
TIM2->CR1&=~(0x3<<TIM_CR1_CKD_Pos);

// включение прерывания в регистре DIER
//TIM2->DIER|=1<<TIM_DIER_UIE_Pos;// прерывание по переполнению
 //TIM2->DIER|=TIM_DIER_CC1IE_Msk;// прервание по сранвнению на Ch1 ()
// Регистры CCMR1/CCMR2 используются для настройки каналов на вход/выход

//CCMR1 каналы 1, 2 ; CCMR2 - каналы 3, 4
// За поведение канала отвечают биты CCxS,
// остальные биты выставляются в соответсствии с установкой CCxS
// ССxS записывается когда соответствующее значение в TIMx_CCER отключено,
 	 // CC2S =
	// 00 -  CC2 chanel is configured as output
	// 01 - CC2 chanel is configured as input, I2C is mapped on TI2
	//10 - CC2 chanel is configured as input, I2C is mapped on TI1
  	// 11 - CC2 chanel is configured as input, I2C is mapped on TCR ,
    //this mode is working only if  an internal trigger, input is selected  through the TS bit

//TIMxCNT
TIM2->PSC =  720-1;//1440;//7199; //1440 = 720*2
TIM2->ARR = 2000;//1000;//199;  // 50 000 в секунду, но т.к. мы срабатываем раз в 20 мс = 1000
TIM2->CCR1=165;
// для серво привода устанавливается регистр сравнения от 6 (0.6 мс) до 20(2 мс)
// для серво s3003 управление от 388 (0 град) мкс - 1264(90 град) мкс - 2140 мкс (180 град)
// от 97 (0) до 316 (90), 535 ( 180)  в регистр сравнения
// для позиционирования по градусам = 97+ [0...180]*2,5
// 931 (0 град)   -- 3034 (90 град) =5136 (180 град)
//23,4 = 1 град
NVIC_EnableIRQ(TIM2_IRQn);

// запуск таймера
// настройка PWM:
 // Альтернативная функция push-pull, mode =11
 // == 1011
// дЛя chanal=1 PA0
// включить тактирование порта a
RCC->APB2ENR|=(1<<RCC_APB2ENR_IOPAEN_Pos);

GPIOA->CRL&=~(0xF<<GPIO_CRL_MODE0_Pos);
GPIOA->CRL|=(0xB<<GPIO_CRL_MODE0_Pos);

// настройка pwm:
TIM2->CR1&=~(0x3<<TIM_CR1_CMS_Pos);
 TIM2->CR1|=(1<<TIM_CR1_ARPE_Pos);
TIM2->CCMR1&=~(0x3<<TIM_CCMR1_CC1S_Pos); // установить канал 1 на выход
 TIM2->CCMR1|=(0x1<<TIM_CCMR1_OC1PE_Pos);
TIM2->CCMR1&=~(0x7<<TIM_CCMR1_OC1M_Pos);
TIM2->CCMR1|=(0x7<<TIM_CCMR1_OC1M_Pos); // PWM_MODE 1

//TIM2->CCMR1&=~(0x1<<TIM_CCMR1_OC1CE_Pos);

TIM2->EGR=(1<<TIM_EGR_UG_Pos);

// запуск Шим
TIM2->CCER|=(1<<TIM_CCER_CC1E_Pos);
TIM2->CCER|=(1<<TIM_CCER_CC1P_Pos);


Tim_start(TIM2);

 }


void PWM_Tim2_cfg()
{
	  RCC->APB2ENR  |= RCC_APB2ENR_IOPAEN;            // Включаем тактирование порта A
	//  GPIOA->CRL    |= GPIO_CRL_CNF0_1;       // Включаем альтернативную функцию на выход с Push/Pull
	 // GPIOA->CRL    |= (GPIO_CRL_MODE0_0 | GPIO_CRL_MODE0_1);
	  RCC->APB2ENR|=(1<<RCC_APB2ENR_AFIOEN_Pos);
	  GPIOA->CRL&=~(0xF<<GPIO_CRL_MODE0_Pos);
	  GPIOA->CRL|=(0xB<<GPIO_CRL_MODE0_Pos);

	  RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
TIM2->PSC=36-1;
TIM2->ARR=40000-1;
TIM2->CCR1 =3040;
TIM2->CCMR1&=~(0x7<<TIM_CCMR1_OC1M_Pos);
TIM2->CCMR1|=(0x6<<TIM_CCMR1_OC1M_Pos);

TIM2->CCMR1&=~(0x3<<TIM_CCMR1_CC1S_Pos); // установить канал 1 на выход
 TIM2->CCMR1|=(0x1<<TIM_CCMR1_OC1PE_Pos);
TIM2->CCER    |= TIM_CCER_CC1E;     // Разрешаем первому каналу плеваться на выход
TIM2->DIER|=1<<TIM_DIER_UIE_Pos;
NVIC_EnableIRQ(TIM2_IRQn);
 // TIM2->CCER|=(1<<TIM_CCER_CC1P_Pos);
  TIM2->CR1     |= TIM_CR1_CEN;
}
void PWM_update(int a)
{
 if(a<0)
 {
TIM2->CCR1-=(uint32_t) abs(a);
 }
 else TIM2->CCR1+=(uint32_t) abs(a);

}

void TIM_config()
{
 // CR1->CEN автоматически сбрасывается в одноимпульсном режиме
// Для включения канала используется TIMx->CCER, биты CCxE - включение, CCxP = конфигурация
//  перед этим нужно определить канал как вход или выход

	//можно проверить что таймер включен
//	настройка выхода PA1

	 GPIOA->CRL&=~(0xF<<GPIO_CRL_MODE1_Pos);
	 GPIOA->CRL|=(0xB<<GPIO_CRL_MODE1_Pos);

	// TIM2->CCER&=~(0x3<<TIM_CCER_CC2E_Pos); // скинуть биты конфигурации
	TIM2->CCMR1&=~(0x3<<TIM_CCMR1_CC2S_Pos); //установить как выход

//TIM2->CCER|=(0x1<<TIM_CCER_CC2E_Pos);// разрешить подачу сигнала на пин при совпадении
TIM2->CCMR1&=~(0x7<<TIM_CCMR1_OC2M_Pos);
TIM2->CCMR1|=(0x2<<TIM_CCMR1_OC2M_Pos);
TIM2->CCER|=(1<<TIM_CCER_CC2P_Pos);
//TIM2->CCR2 =10;
// TIM2->CR1|=1<<TIM_CR1_OPM_Pos;

}

void set_impulse(uint16_t imp)
{

//TIM2->DIER|=1<<TIM_DIER_CC2IE; // включаем прерывание по второму каналу


	//TIM2->CCER|=(0x1<<TIM_CCER_CC2E_Pos);
	TIM2->CCR2=(uint16_t)(imp+TIM2->CNT);
	// в обработчике нужно отключить прерывание, и канал от вывода
 TIM2->CCER|=(0x1<<TIM_CCER_CC2E_Pos);
	TIM2->SR&= ~(TIM_SR_CC2IF);
	while (!(TIM2->SR & TIM_SR_CC2IF)); // возможно перенести в прерывание по compare
   TIM2->CCER&=~(0x1<<TIM_CCER_CC2E_Pos);
	// TIM2->SR&= ~(TIM_SR_CC2IF);


}

void count_delay_config()
{
 // для echo нужно сканфигурировать пин на вход,(с подтяжкой или без),
	// нужна еще гальваническая развязка 3.3 -5 в
	//
	// допустим подключаем на эхо PA2
//Для этого нужно сконфигурировать PA2 как вход, альтернативную функцию
	RCC->APB2ENR|=(RCC_APB2ENR_IOPAEN); // включить тактирование порта
	 RCC->APB2ENR|=(RCC_APB2ENR_AFIOEN); // включить альтернативнцю функцию
// Для внешних прерываний не требуется альтернатиыной функции
GPIOA->CRL&=~(0x7<<GPIO_CRL_MODE2_Pos);
	 //GPIOA->CRL|=(0x8<<GPIO_CRL_MODE2_Pos); // установить как вход =просто вход,с подтяжкой
	//подтяжка внешняя задается
	   GPIOA->CRL|=(0x4<<GPIO_CRL_MODE2_Pos); //и требует установки внешнего резистора
 GPIOA->ODR|=(1<<GPIO_ODR_ODR2_Pos); //установить  pull-down подтяжку

//Регистр событий сюда пока не лезем этого AFIO->EVCR подключаем PA2
	//AFIO->EVCR&=~(0x7<<AFIO_EVCR_PORT_Pos); // сбросить значение порта для выбора GPIOA
//AFIO->EVCR |=(0x4<<AFIO_EVCR_PIN_Pos);  // установить PA2

	// настройка Exti2 прерываний по нарастанию и спаду фронта
// в качестве источника прерываний exti выбираем PA2
	AFIO->EXTICR[0] &=~( AFIO_EXTICR1_EXTI2 ); // EXTICR определен в качестве массива, из 4х регистров
		// каждый из регистров определяет соответствующий вывод для заданного порта
	// EXTICR1 определяет выводы 0-3 для заданных портов,
	// например для включения прерывания на втором пине порта x, в AFIO_EXTICR1_EXTI2 записывается комбинация  порта
	// стр 190
	NVIC_EnableIRQ(EXTI2_IRQn);
// определяем прерыания по спаду и возрастанию
	EXTI->RTSR|=EXTI_RTSR_TR2;
	EXTI->FTSR|=EXTI_FTSR_TR2;

	// далее сбросить флаг прерывания и разрешить внешнее прерывание

 // EXTI->PR=EXTI_PR_PR2;
// 	EXTI->IMR|=(EXTI_IMR_IM2);
// ДОбавляем прерывание в контроллер

}


 void config_usart1 () //enum boudrate bd) // (int num,
 {

 // настройка usart1
     	 RCC->APB2ENR |=RCC_APB2ENR_USART1EN;
    // Usart1 подключен к выводам a9,a10 нужна конфигурация для них
     	 RCC->APB2ENR|=(1<<RCC_APB2ENR_IOPAEN_Pos); // тактирование порта

    // тактирование альтернативных функции
     	 RCC->APB2ENR|=(1<<RCC_APB2ENR_AFIOEN_Pos);

     	 // далее настройка выводов a9 a10 на альтернативную функцию
     	 GPIOA->CRH&=~ (0xF<<GPIO_CRH_MODE10_Pos|0xF<<GPIO_CRH_MODE9_Pos);
     	   // очистить настройки a9, a10;
 // Вывод a9 (transmision TX)
     	  // настройка на альтернативнцю функцию с активным выходом CFG [1:0] MOD[1:1]
 GPIOA->CRH|=(0x2<<GPIO_CRH_CNF9_Pos|GPIO_CRH_MODE9);

  // Выход a10 настраивается на вход с подтяжкой
  // CNF [1:0] MOD [0:0]
 GPIOA->CRH|=(0x2<<GPIO_CRH_CNF10_Pos);
 GPIOA->BSRR|=GPIO_ODR_ODR10;

 // конфигурация usart
 USART1->CR1=USART_CR1_UE;

 USART1->BRR=0x753;
 //USART1->DR=0;
 USART1->CR1|=(USART_CR1_RE|USART_CR1_TE |USART_CR1_RXNEIE);

 USART1->CR2=0;
 USART1->CR3=0;
 NVIC_EnableIRQ(USART1_IRQn);

 }
