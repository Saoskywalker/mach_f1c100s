#ifndef _PWM_H
#define _PWM_H

#include "types_plus.h"

typedef struct
{
	dev_addr addr; //设备地址
	float duty; //占空比 0~100%
	float period; //频率 KHz
	uint8_t polarity; //极性 0正常(高电平和占空比一致), 1翻转
    uint8_t state; //设备状态 
} pwm_dev_type;

char MTF_pwm_init(pwm_dev_type *dev);
char MTF_pwm_exit(pwm_dev_type *dev);
void MTF_pwm_start(pwm_dev_type *dev);
void MTF_pwm_suspend(pwm_dev_type *dev);
void MTF_pwm_set_duty(pwm_dev_type *dev, float i);

#endif
