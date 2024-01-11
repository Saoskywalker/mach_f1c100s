/*
Copyright (c) 2019-2023 Aysi 773917760@qq.com. All right reserved
Official site: www.mtf123.club

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

It under the terms of the Apache as published;
either version 2 of the License,
or (at your option) any later version.
*/

#include "timer_port.h"
#include "timer_f1c100s.h"
#include "irq_misc.h"
#include "Sagittarius_timer.h"

/*
定时器中断
*/
void TIMER0_ISR(void *arg)
{
	// TEST_TOGGLE_PIN();
    Sagittarius_timer();
	TIMER0_CLEAR(); //清中断
}

void TIMER1_ISR(void *arg)
{
	// TEST_TOGGLE_PIN();
	TIMER1_CLEAR(); //清中断
}

void TIMER2_ISR(void *arg)
{
	// TEST_TOGGLE_PIN();
	TIMER2_CLEAR(); //清中断
}

void MTF_timer_init_handle(void)
{
    Timer_Init(TIMER0, 1500, 1);
	request_irq(IRQ_TIMER0, TIMER0_ISR, 0); //register int
	Timer_enable(TIMER0);
}
