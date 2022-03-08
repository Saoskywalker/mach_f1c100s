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
