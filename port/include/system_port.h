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

#ifndef _SYSTEM_PORT_H
#define _SYSTEM_PORT_H

#include "types_base.h"

void MTF_sys_reset(void);
uint8_t system_exit_weak(void);
uint8_t system_process_weak(void);

uint8_t hardware_init_weak(void);

typedef uint8_t MTF_mutex;

MTF_mutex *MTF_CreateMutex(void);

static inline void MTF_DestroyMutex(MTF_mutex *mutex)
{
    return;
}

static inline int MTF_TryLockMutex(MTF_mutex *mutex)
{
    return 0;
}

static inline int MTF_LockMutex(MTF_mutex *mutex)
{
    return 0;
}

static inline int MTF_UnlockMutex(MTF_mutex *mutex)
{
    return 0;
}

#endif
