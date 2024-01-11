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

#include "PWM_port.h"
#include <pwm-f1c100s.h>

static pwm_t Beep_PIN = {
    0x01c21000,
    "",
    100,
    250000, //100000: 10K; 1000000:1K; 250000 4K
    1,
    1,
    &GPIO_PF,
    5,
    4};

char MTF_pwm_init(pwm_dev_type *dev)
{
    Beep_PIN.duty = (int32_t)dev->duty;
    Beep_PIN.period = 250000;
    Beep_PIN.polarity = !dev->polarity;
    dev->state = 0;
    return (char)pwm_f1c100s_init(&Beep_PIN);
}

char MTF_pwm_exit(pwm_dev_type *dev)
{
    dev->state = 0;
    pwm_f1c100s_disable(&Beep_PIN);
    return 0;
}

void MTF_pwm_start(pwm_dev_type *dev)
{
    dev->state = 1;
    pwm_f1c100s_enable(&Beep_PIN);
}

void MTF_pwm_suspend(pwm_dev_type *dev)
{
    dev->state = 0;
    pwm_f1c100s_disable(&Beep_PIN);
}

void MTF_pwm_set_duty(pwm_dev_type *dev, float i)
{
    dev->duty = i;
    pwm_f1c100s_set_duty(&Beep_PIN, (int32_t)i);
}
