#include "support.h"



void delay_echo::set_pin()
{//uint32_t reg=GPIOA->IDR;
 if( GPIOA->IDR & (1<<GPIO_IDR_IDR2_Pos))
	 set_rise();
 else //if( reg & (1<<GPIO_IDR_IDR2_Pos)==0)
	 set_fall();
  }
void delay_echo::set_value(volatile uint32_t & v1,volatile uint32_t &v2) //
{
if(cnt_reg&(1<<up)) // если нарастающий фронт, складываем с текущими
{

	   cnt_val=v1;// сохраняем значение tim2_cnt;
		 TIM_cnt_val=v2;

}
 // если фронт спадающий
 if(cnt_reg & (1<<down))
 {
	 addition(v1, v2);
	    cnt_val=(v1-cnt_val);
	    TIM_cnt_val=(v2>TIM_cnt_val)?v2-TIM_cnt_val:(0xFFFF-TIM_cnt_val)+v2;
	 	set_ready();

 }
}
void delay_echo::set_dist()
{ //dist=0;

	  dist=((cnt_val*20000)+(TIM_cnt_val/2));
	 cnt_val=TIM_cnt_val=cnt_reg=0;
  //   cnt_reg&=~(1<<delay_echo::ready);
	}
void delay_echo::set_rise()
   { // cnt_reg&=~(1<<ready);
	  dist=0;
	 cnt_reg&=~(1<<down) ; // сбросить флаг по спаду
     cnt_reg|=(1<<up);
   }

 void delay_echo::set_fall()
   {
	 cnt_reg&=~(1<<up);
	 cnt_reg|=(1<<down);
	// set_ready();
	   }

