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
