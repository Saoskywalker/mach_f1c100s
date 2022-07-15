#ifndef _TOUCH_DEV_H
#define _TOUCH_DEV_H

#include "types_base.h"

uint8_t touch_init(void); //初始化触摸屏控制器
uint8_t touch_exit(void); //移除触摸屏控制器
uint8_t touch_scan(int *x, int *y, uint8_t target_num, uint8_t *result_num); //扫描触摸屏

#endif
