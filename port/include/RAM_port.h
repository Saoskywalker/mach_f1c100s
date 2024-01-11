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

#ifndef _RAM_PORT_H
#define _RAM_PORT_H

#include "types_base.h"
#include <malloc.h>
#include "dma_pool.h"

static __INLINE char MTF_RAM_init(void)
{
    /* Do initial mem pool */
	do_init_mem_pool();
    do_init_dma_pool();
    return 0;
}

#endif
