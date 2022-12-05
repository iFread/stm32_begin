#include "stm32f103x6.h"
#include "init.h"
#include "paket.h"
#include "support.h"
volatile uint32_t  tst_val=0;
  volatile uint8_t sec=0;
  volatile uint32_t tim2_cnt=0;
  volatile uint32_t dist=0;
  volatile bool up=false;
uint8_t buffer[64]={0};
common::pak_v2 pak(64,buffer);


extern "C"
 void SysTick_Handler() {
tst_val++;

	if(tst_val==1000){
if(sec%2>0)
	 {
	 GPIOC->BSRR|=(1<<GPIO_BSRR_BS13_Pos);// ODR|=(1<<GPIO_ODR_ODR13_Pos);//BSRR|=(1<<GPIO_BSRR_BS13_Pos);
	  //tst_val=0;
	 	  }

else
{

	 GPIOC->BSRR|=(1<<GPIO_BSRR_BR13_Pos);
//GPIOC->ODR&=~(1<<GPIO_ODR_ODR13_Pos);
//trace_printf("=%d sec",  sec) ;
}
sec++;
tst_val=0;

	}}
extern "C"
void  USART1_Handler()
{
	//trace_printf("Hello\n")	;
uint8_t dd;
	 dd=USART1->DR;

pak.rc_nxt(&dd);
	}

extern "C"
void TIM2_Handler()
{
	 if( TIM2->SR& TIM_SR_UIF)
	 {
	 TIM2->SR&=~(1<<TIM_SR_UIF_Pos);
//	 if(tim2_cnt%2>0)
//	     GPIOC->ODR|=(1<<GPIO_ODR_ODR15_Pos);
//	 else
		 GPIOC->ODR^=(1<<GPIO_ODR_ODR15_Pos);


	 //if( TIM2->SR& TIM_SR_CC1OF_Pos)
	 //	 {
	 //	 TIM2->SR&=~(1<<TIM_SR_CC1OF_Pos);
if((tim2_cnt++)%3==0){

	 if(up)
	 	 {
	 	 	PWM_update(243);
	 	//  TIM2->CCR1=3530;
	 	  if(TIM2->CCR1 >=6650)// TIM2->ARR-2)
	 	    up=false;

	 	 }
	 	 else
	 	  {
	 			 PWM_update(-243);
	 		// TIM2->CCR1=570;
	 	 if(TIM2->CCR1 <= 550)// TIM2->ARR)
	 	          up=true;
	 	  }


	// tim2_cnt=0;
  }
// каждое третье прерывание =60 мс,
// запускать функцию измерения hc- sr04
// -- создать импульс 10 мкс,
// посчитать сигнал echo
// перевести в мм и отправить по usart
//	if(tim2_cnt%100==0)
//	{
//		 set_impulse(20);
//	}
	 }
// обработка второго канала

//if(TIM2->SR & TIM_SR_CC2IF)
//{
//	TIM2->SR&=~(1<<TIM_SR_CC2IF_Pos);
//	TIM2->DIER&=~(1<<TIM_DIER_CC2IE_Pos); // отключить прерывание
//    TIM2->CCER&=~(1<<TIM_CCER_CC2E_Pos);// отключить вывод
//	EXTI->IMR|=(EXTI_IMR_IM2);
//}
}
// обработчик внешнего прерывания
// сохраняем значение регистра TIM2_CNT
// и регистра tim2_cnt;
// зная что прерывания выполняются каждые 20 мс, посчитаем задержку
struct delay_echo st1;

extern "C"
void EXTI2_Handler()
{

	 // 0. сбросить флаг прерывания
EXTI->PR|= (1<<EXTI_PR_PR2_Pos);
// проверить какое прерывание сработало,
st1.set_pin();
st1.set_value(tim2_cnt, TIM2->CNT);
 // 1. если по возрастанию считыаем значения в структуру
	 //2. если по убыванию - считаем разницу значений
if(st1.cnt_reg & 1<<delay_echo::down)
	EXTI->IMR&=~(1<<EXTI_IMR_MR2_Pos);
}



int main ()
{
	 configure_sysclock();
		 Sys_Tick_config();
	      Sys_Tick_value(72000000/1000-1);
	 	   Sys_Tick_start();
	 	  config_usart1();
	 	//  NVIC_EnableIRQ(USART1_IRQn);
	 	 PWM_Tim2_cfg();
	 	 TIM_config();

	 	  count_delay_config();
	 	 uint8_t del=0;
	 	 uint8_t fl_imp=1;
	 	 uint32_t ctrl=0;
	 	  while(1)
	{
	 		 if(tim2_cnt%3==0 && !fl_imp )
	 		 { ctrl=tim2_cnt;
	 		 	 fl_imp=1;
	 		 	 set_impulse(20);
	 		 	 EXTI->IMR|=(EXTI_IMR_IM2);
	 		 }
//	 		 	 }
	 		 else if((ctrl!=tim2_cnt))
	 		 	   {
	 		 		  fl_imp=0;
	 		 	   }
	 //	 if(sec!=del)
	// 		{ // каждую секунду активируем прерывания по фронту и спаду
	// 	del=sec;
	 		 //	EXTI->RTSR|=EXTI_RTSR_TR2;
	 		//	EXTI->FTSR|=EXTI_FTSR_TR2;

	 			//EXTI->IMR|=(1<<EXTI_IMR_MR2_Pos);
	 		//	 while(!(st1.cnt_reg & (1<<delay_echo::ready)))
	 		//;


if(st1.cnt_reg&1<<delay_echo::ready)
{		st1.set_dist();
	 	dist=st1.dist;

	 		 pak.write((uint8_t*)&dist, sizeof(dist));
	 		// uint8_t buf[]={"hello world"};
	 		uint8_t d=0;
	 		 if(pak.isReady())
	 		for(uint8_t i=0;i<pak.size();i++){
	 			while((USART1->SR & USART_SR_TXE)==0) {}
	 		   if(pak.tr_nxt(&d))
	 			USART1->DR= d;//buf[i];//(pak>> (i-1)*8); //24--16--8 --0
	 		}
	 		pak.clear();
	 		 }
	 		}
	// }


return 0;
 }
