// вспомогательные структуры данных

#include "stm32f103x6.h"

#ifndef _SUPPORTING_H_
#define _SUPPORTING_H_


struct delay_echo
{ // старшая тетрада в cnt_reg отвечает за количество измерений,
	// таким образом можно хранить до 15 замеров
	// check_cnt содержит количество измерений, beg_cnt - начало счетчика , т.е. количество измерений
	// будет записываться в регистр cntl_reg  начиная с 4 го бита
	enum control {ready,rise,down,check_cnt=3, beg_cnt=4};
	uint32_t cnt_val;       // значение счетчика прерываний, т.к.
	   // частота 50Гц, для чханения этого значения достаточно 8 байт
 	uint32_t TIM_cnt_val; // значение таймера прерываний
 	 	 	 // таймер не привышает uint16_t, для подсчета среднего используем uint32_t
 	 uint32_t dist;
 // 	нужен буфер для хранения промежуточных значений
 uint32_t b_isr; // буфер для хранения количества прерываний
 uint16_t b_TIM;  // буфер для хранения

 // вариант второй = считать среднюю дистанцию


// установочный регистр
	uint8_t ctrl_reg;

	delay_echo():cnt_val(0),TIM_cnt_val(0),dist(0),b_isr{0},b_TIM{0},ctrl_reg{0}
 {
  }
 inline void set_ready()
 {
	  ctrl_reg|=(1<<ready);
 }
// маркирует прерывание по возрастанию
    void set_rise();
//   { cnt_reg&=~(1<<ready);
//	 // dist=0;
//	cnt_reg&=~(1<<down) ; // сбросить флаг по спаду
//     cnt_reg|=(1<<up);
//   }
bool st_rise(){ return ctrl_reg& (1<<rise);}
bool st_down(){return ctrl_reg& (1<<down);}
bool st_ready(){return ctrl_reg&(1<<ready);}
void set_fall();
//   {
//	 cnt_reg&=~(1<<up);
//	 cnt_reg|=(1<<down);
//	   }

    void set_pin();

    void set_value(volatile uint32_t & TC,volatile uint32_t& tc); // принимает по ссылке значения счетчиков
    void set_dist();
private:
 void addition(volatile uint32_t &TC,volatile uint32_t &tc);

};


#endif
