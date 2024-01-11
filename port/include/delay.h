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

#ifndef _DELAY_H
#define _DELAY_H

#include <stdint.h>

void delay(volatile uint32_t ms);
void delay_us(volatile uint32_t us);
void delay_ms(volatile uint32_t ms);
void delay_always_init(void);
void delay_always(uint32_t ms);

#if defined(__CC_ARM)

static __inline void sdelay(int loops)
{
	delay_us(loops);
}

#else

static inline void sdelay(int loops)
{
	__asm__ __volatile__ ("1:\n" "subs %0, %1, #1\n"
		"bne 1b":"=r" (loops):"0"(loops));
}

#endif

#endif
