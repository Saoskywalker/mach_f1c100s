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

#ifndef _WATCH_DOG_PORT_H
#define _WATCH_DOG_PORT_H

#include "wdog-f1c100s.h"
#include "types_base.h"

static __INLINE void MTF_watch_dog_init(void) //start WDOG
{
    wdog_f1c100s_set(WDOG_14S);
}

static __INLINE void MTF_watch_dog_feed(void) //feed WDOG
{
    wdog_f1c100s_feed(WDOG_14S);
}

#endif
