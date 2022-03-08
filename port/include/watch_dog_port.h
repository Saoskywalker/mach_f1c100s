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
