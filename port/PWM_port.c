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

char MTF_pwm_init(PWM_dev *dev)
{
    Beep_PIN.duty = (int32_t)dev->duty;
    Beep_PIN.period = 250000;
    Beep_PIN.polarity = !dev->polarity;
    dev->state = 0;
    return (char)pwm_f1c100s_init(&Beep_PIN);
}

char MTF_pwm_exit(PWM_dev *dev)
{
    dev->state = 0;
    pwm_f1c100s_disable(&Beep_PIN);
    return 0;
}

void MTF_pwm_start(PWM_dev *dev)
{
    dev->state = 1;
    pwm_f1c100s_enable(&Beep_PIN);
}

void MTF_pwm_suspend(PWM_dev *dev)
{
    dev->state = 0;
    pwm_f1c100s_disable(&Beep_PIN);
}

void MTF_pwm_set_duty(PWM_dev *dev, float i)
{
    dev->duty = i;
    pwm_f1c100s_set_duty(&Beep_PIN, (int32_t)i);
}
