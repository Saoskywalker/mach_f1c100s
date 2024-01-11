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
