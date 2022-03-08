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
} PWM_dev;

char MTF_pwm_init(PWM_dev *dev);
char MTF_pwm_exit(PWM_dev *dev);
void MTF_pwm_start(PWM_dev *dev);
void MTF_pwm_suspend(PWM_dev *dev);
void MTF_pwm_set_duty(PWM_dev *dev, float i);

#endif
