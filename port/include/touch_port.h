#ifndef _TOUCH_DEV_H
#define _TOUCH_DEV_H

#include "types_base.h"

char touch_init(void); //初始化触摸屏控制器
char touch_exit(void); //移除触摸屏控制器
uint8_t touch_scan(uint8_t i); //扫描触摸屏

#endif
