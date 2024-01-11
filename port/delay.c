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

#include "delay.h"

#define CPU_FREQUENCY 720 //CPU: 720M

#if CPU_FREQUENCY == 720
#define _MS_CNT 90000
#define _US_CNT 87
#else
#define _MS_CNT 51000
#define _US_CNT 36
#endif

void delay(volatile uint32_t ms)
{
	while(ms--)
	{
		volatile uint32_t n = _MS_CNT;
		while(n--);
	}
}

void delay_us(volatile uint32_t us)
{
	while(us--)
	{
		volatile uint32_t n = _US_CNT;
		while(n--);
	}
}

void delay_ms(volatile uint32_t ms)
{
	while(ms--)
	{
		volatile uint32_t n = _MS_CNT;
		while(n--);
	}
}

