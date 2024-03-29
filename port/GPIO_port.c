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

#include "GPIO_port.h"

void MTF_GPIO_Init(MTF_GPIO_PORT *GPIOx, MTF_GPIO_Type *GPIO_Init)
{

    if (GPIO_Init->Mode == MTF_PIN_MODE_INPUT)
    {
        gpio_f1c100s_set_dir(GPIOx, (int32_t)GPIO_Init->Pin, GPIO_DIRECTION_INPUT);
    }
    else
    {
        gpio_f1c100s_set_dir(GPIOx, (int32_t)GPIO_Init->Pin, GPIO_DIRECTION_OUTPUT);
        gpio_f1c100s_set_value(GPIOx, (int32_t)GPIO_Init->Pin, 0); //初始化默认为低电平
    }
    if (GPIO_Init->Pull == MTF_PIN_NOPULL)
        gpio_f1c100s_set_pull(GPIOx, (int32_t)GPIO_Init->Pin, GPIO_PULL_NONE);
    else if (GPIO_Init->Pull == MTF_PIN_PULLUP)
        gpio_f1c100s_set_pull(GPIOx, (int32_t)GPIO_Init->Pin, GPIO_PULL_UP);
    else if (GPIO_Init->Pull == MTF_PIN_PULLDOWN)
        gpio_f1c100s_set_pull(GPIOx, (int32_t)GPIO_Init->Pin, GPIO_PULL_DOWN);
}
