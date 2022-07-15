#ifndef _SYSTEM_PORT_H
#define _SYSTEM_PORT_H

#include "types_base.h"

void MTF_sys_reset(void);

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
