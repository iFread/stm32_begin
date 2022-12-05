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
if(ctrl_reg&(1<<rise)) // если нарастающий фронт, складываем с текущими
{
	   b_isr=v1;// сохраняем значение tim2_cnt;
		 b_TIM=v2; // и значение счетчика

}
 // если фронт спадающий
 if(ctrl_reg & (1<<down))
 {
   //1. проверить что измерение  последнее из требуеиого количества
	  if(ctrl_reg & check_cnt<<beg_cnt) //    3<<4 ==> проверить, что начиная с четвертого бита в ctrl_reg
	  {
		  cnt_val =(cnt_val+1)/(check_cnt<<0);
		  TIM_cnt_val/=(check_cnt<<0);
		  set_ready();
     }
else {
	 addition(v1, v2); // сохранить полученное время
	    cnt_val+=b_isr;//(v1-cnt_val);
	    TIM_cnt_val+=b_TIM; //(v2>TIM_cnt_val)?v2-TIM_cnt_val:(0xFFFF-TIM_cnt_val)+v2;
 // увеличить значение счетчика в регистре ctrl_reg
	    uint8_t cnt=ctrl_reg>>beg_cnt;
	    cnt++;
	    ctrl_reg&=~( check_cnt<<beg_cnt);
	    ctrl_reg|=(cnt<<beg_cnt);
	   }
 }
}

void delay_echo::addition(volatile uint32_t &TC,volatile uint32_t &tc)
{
 b_isr=TC-b_isr;
 b_TIM=(tc>b_TIM)?tc-b_TIM:(0xFFFF-b_TIM)+tc;

}
void delay_echo::set_dist()
{ //dist=0;

	  dist=((cnt_val*20000)+(TIM_cnt_val)/2);
	 cnt_val=TIM_cnt_val=ctrl_reg=0;
  //   cnt_reg&=~(1<<delay_echo::ready);
	}
void delay_echo::set_rise()
   {   ctrl_reg&=~(1<<ready);
	  dist=0;
	 ctrl_reg&=~(1<<down) ; // сбросить флаг по спаду
     ctrl_reg|=(1<<rise);
   }

 void delay_echo::set_fall()
   {
	 ctrl_reg&=~(1<<rise);
	 ctrl_reg|=(1<<down);
	// set_ready();
	   }

