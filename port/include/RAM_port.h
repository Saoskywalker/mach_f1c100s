#ifndef _RAM_PORT_H
#define _RAM_PORT_H

#include "types_base.h"
#include <malloc.h>
#include <dma.h>

static __INLINE char MTF_RAM_init(void)
{
    /* Do initial mem pool */
	do_init_mem_pool();
    do_init_dma_pool();
    return 0;
}

#endif
