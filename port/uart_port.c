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

#include "uart_port.h"

HAL_StatusTypeDef MTF_UART_Init(MTF_HandleDef *huart)
{
    huart->uart = uart_16550_init((uint8_t)huart->addr, (int)huart->Init.BaudRate, (int)huart->Init.WordLength,
                                  (int)huart->Init.Parity, (int)huart->Init.StopBits);
    return HAL_OK;
}

HAL_StatusTypeDef MTF_UART_exit(MTF_HandleDef *huart)
{
    uart_16550_close(&huart->uart);
    return HAL_OK;
}

HAL_StatusTypeDef MTF_UART_Reset(MTF_HandleDef *huart)
{
    uart_16550_fifo_reset(huart->uart);
    return HAL_OK;
}

size_t MTF_UART_Transmit(MTF_HandleDef *huart, uint8_t *pData, size_t Size)
{
    ssize_t i = 0;
    i = huart->uart->write(huart->uart, pData, Size);
    return (size_t)i;
}

size_t MTF_UART_Receive(MTF_HandleDef *huart, uint8_t *pData, size_t Size)
{
    ssize_t i = 0;
    i = huart->uart->read(huart->uart, pData, Size);
    return (size_t)i;
}

uint8_t MTF_UART_Transmit_Empty(MTF_HandleDef *huart)
{
    return uart_16550_send_empty(huart->uart);
}

uint8_t MTF_UART_Receive_Empty(MTF_HandleDef *huart)
{
    return uart_16550_rec_empty(huart->uart);
}
