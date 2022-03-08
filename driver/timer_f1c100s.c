#include <io.h>
#include "timer_f1c100s.h"
#include "irq_misc.h"
#include "stdio.h"

#define TMR_IRQ_EN_REG (0x01C20C00 + 0x00)
#define TMR_IRQ_STA_REG (0x01C20C00 + 0x04)
#define TMR0_CTRL_REG (0x01C20C00 + 0x10)
#define TMR0_INTV_VALUE_REG (0x01C20C00 + 0x14)
#define TMR0_CUR_VALUE_REG (0x01C20C00 + 0x18)

/* 
void TIMER0_init(u32 value, u32 pres)
{
	write32(TMR_IRQ_EN_REG, 1);
	write32(IRQ_ADDR_BASE + IRQ_ENABLE0, (read32(IRQ_ADDR_BASE + IRQ_ENABLE0)) | 1 << 13);
	write32(TMR_IRQ_EN_REG, (read32(TMR_IRQ_EN_REG)) | 1 << 13);
	write32(TMR0_INTV_VALUE_REG, value * 24); //Set interval value  set 20:1ms 2:150us
	write32(TMR0_CTRL_REG, 0);
	write32(TMR0_CTRL_REG, read32(TMR0_CTRL_REG) | (pres << 4));
	write32(TMR0_CTRL_REG, read32(TMR0_CTRL_REG) | (1 << 2)); //there24M losc:about 36k
	write32(TMR0_CTRL_REG, read32(TMR0_CTRL_REG) | (1 << 1)); //Set Reload bit
	while ((read32(TMR0_CTRL_REG) >> 1) & 1);		 //Waiting Reload bit turns to 0
	write32(TMR0_CTRL_REG, read32(TMR0_CTRL_REG) | (1 << 0)); //Enable Timer0

	request_irq(IRQ_TIMER0, TIMER0_DEMO_ISR, 0); //register int
}
 */

/*******************************************************/
/*
定时器初始化
timer=TIMER0,TIMER1,TIMER2【f1c100s有三个32位定时器】
IRQ_EN=中断使能
time_us=中断时间 us 最大178956970
*/
void Timer_Init(u8 timer, u32 time_us, u8 IRQ_EN)
{
	u32 val = 0;
	write32(TIMER_CTRL(timer), 0);
	//Timer 模式 0连续 1单
	val = 0;
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 7));
	//Timer 分频
	val = 0;
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 4));
	//Timer 时钟源
	val = 1; //there is 24Mhz , losc about 36k
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 2));
	//Timer 重装计数值
	val = 1; //=1重装计数值
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 1));

	//Timer 计数值
	val = 24 * time_us;
	write32(TIMER_INTV(timer), val);

	//使能中断
	if (IRQ_EN == 1)
	{
		write32(TIMER_IRQ_EN, read32(TIMER_IRQ_EN) | (1 << timer));
	}
}
/*
定时器开
*/
void Timer_enable(u8 timer)
{
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((1) << 0));
}
/*
定时器关
*/
void Timer_disable(u8 timer)
{
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) & (~((1) << 0)));
}
/*
定时器中断
*/
void _count_timer(void);
void TIMER0_CLEAR(void)
{
	write32(TIMER_IRQ_STA, read32(TIMER_IRQ_STA) | (1 << TIMER0)); //清中断
}
void TIMER0_DEMO_ISR(void *arg)
{
	TIMER0_CLEAR(); //清中断
	// TEST_TOGGLE_PIN();
	_count_timer();
}

void TIMER1_CLEAR(void)
{
	write32(TIMER_IRQ_STA, read32(TIMER_IRQ_STA) | (1 << TIMER1)); //清中断
}
void TIMER1_DEMO_ISR(void *arg)
{
	// TEST_TOGGLE_PIN();
	TIMER1_CLEAR(); //清中断
}

void TIMER2_CLEAR(void)
{
	write32(TIMER_IRQ_STA, read32(TIMER_IRQ_STA) | (1 << TIMER2)); //清中断
}
void TIMER2_DEMO_ISR(void *arg)
{
	// TEST_TOGGLE_PIN();
	TIMER2_CLEAR(); //清中断
}

/*
读定时器计数
*/
static unsigned int sys_count_timer0 = 0;
void _count_timer(void)
{
	sys_count_timer0++;
	if (sys_count_timer0 >= 0xffffffff)
		sys_count_timer0 = 0;
}
unsigned int Read_time_ms(void)
{
	return sys_count_timer0;
}
/*
延时函数(定时器)
*/
void Tdelay_ms(int ms)
{
	unsigned int time = 0;
	time = sys_count_timer0;
	while ((sys_count_timer0 - time) < ms);
}
void _timer_delay_us(u32 time_us)
{
	u8 timer = TIMER2;
	u32 val = 0;
	write32(TIMER_CTRL(timer), 0);
	// val = 0; //Timer 模式 0连续 1单
	// write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 7));
	// val = 0; //Timer 分频
	// write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 4));
	val = 1; //Timer 时钟源 24Mhz
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 2));
	// val = 1; //=1重装计数值
	// write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((val) << 1));
	val = 24 * time_us; //Timer 计数值
	write32(TIMER_INTV(timer), val);
	// write32(TIMER_CUR(timer), val);
	//write32(TIMER_IRQ_EN,read32(TIMER_IRQ_EN)	&	(~(1<<timer))); //关中断
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) | ((1) << 0)); //enable
	while ((read32(TIMER_IRQ_STA) & (1 << timer)) == 0); //检查标志
	write32(TIMER_IRQ_STA, read32(TIMER_IRQ_STA) | (1 << timer));		 //清标志
	write32(TIMER_CTRL(timer), read32(TIMER_CTRL(timer)) & (~(1) << 0)); //disable
}
/*
定时器测试
*/
void Timer_Demo(void)
{
	// gpio_f1c100s_set_dir(&GPIO_PE, 7, GPIO_DIRECTION_OUTPUT);
	Timer_Init(TIMER0, 1500, 1);
	request_irq(IRQ_TIMER0, TIMER0_DEMO_ISR, 0); //register int
	Timer_enable(TIMER0);
	// Timer_Init(TIMER1, 200000, 1);
	// request_irq(IRQ_TIMER1, TIMER1_DEMO_ISR, 0); //register int
	// Timer_enable(TIMER1);
	//	while(1)
	//	{
	//		printf("TIMER %d \r\n",sys_count_timer0);
	//		Tdelay_ms(1000);
	//	}
}

/*音视频同步时间初始化-100us*/
void AVS_Time_Init(void)
{
	/*24M时钟通过*/
	S_BIT(0x01C20000 + 0x0144, 31);
	/*初始为100us/次*/
	write32(F1C100S_TIMER_BASE + 0x8c, (2400 - 1) << 0 | (2400 - 1) << 16);
}
/*停止计数器*/
void AVS_Time_Stop(int AVS_TIME_Inx)
{
	if (AVS_TIME_Inx == AVS_TIME_0)
	{
		write32(F1C100S_TIMER_BASE + 0x84, 0);
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) & (~((1) << 0)));
	}
	if (AVS_TIME_Inx == AVS_TIME_1)
	{
		write32(F1C100S_TIMER_BASE + 0x88, 0);
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) & (~((1) << 1)));
	}
}
/*开始并计数器清0*/
void AVS_Time_Start(int AVS_TIME_Inx)
{
	if (AVS_TIME_Inx == AVS_TIME_0)
	{
		write32(F1C100S_TIMER_BASE + 0x84, 0);
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) | (1) << 0);
	}
	if (AVS_TIME_Inx == AVS_TIME_1)
	{
		write32(F1C100S_TIMER_BASE + 0x88, 0);
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) | (1) << 1);
	}
}
/*读计数值 33位计数器读出的为高32位*/
unsigned int AVS_Time_Read(int AVS_TIME_Inx)
{
	if (AVS_TIME_Inx == AVS_TIME_0)
		return read32(F1C100S_TIMER_BASE + 0x84) * 2;
	if (AVS_TIME_Inx == AVS_TIME_1)
		return read32(F1C100S_TIMER_BASE + 0x88) * 2;
	return 0;
}
/*计数暂停*/
void AVS_Time_Pause(int AVS_TIME_Inx)
{
	if (AVS_TIME_Inx == AVS_TIME_0)
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) | (1) << 8);
	if (AVS_TIME_Inx == AVS_TIME_1)
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) | (1) << 9);
}
/*计数继续*/
void AVS_Time_Recover(int AVS_TIME_Inx)
{
	if (AVS_TIME_Inx == AVS_TIME_0)
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) & (~((1) << 8)));
	if (AVS_TIME_Inx == AVS_TIME_1)
		write32(F1C100S_TIMER_BASE + 0x80, read32(F1C100S_TIMER_BASE + 0x80) & (~((1) << 9)));
}
