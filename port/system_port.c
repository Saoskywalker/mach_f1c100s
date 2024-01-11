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

#include "types_base.h"
#include "MTF_io.h"
#include "ROM_port.h"
#include "uart_port.h"
#include "system_port.h"
#include "framebuffer_port.h"

#define DEBUG_SYSTEM(...) //printf(__VA_ARGS__)

void MTF_sys_reset(void)
{
    DEBUG_SYSTEM("sys will reset ...\r\n");
}

//注意, 此方式仅单核下, 双线程, 线程间非固定时间片调度使用
static uint8_t MutexTable[10] = {0}, MutexCnt = 0;
static uint8_t MutexPause = 0;
MTF_mutex *MTF_CreateMutex(void)
{
    if(MutexPause)
        return NULL;
    else
        MutexPause = 1;

    MutexCnt++;
    if (MutexCnt>=sizeof(MutexTable))
    {
        MutexCnt--;
        MutexPause = 0;
        return NULL;
    }
    MutexPause = 0;
    MutexTable[MutexCnt] = 0;
    return &(MutexTable[MutexCnt]);
}
