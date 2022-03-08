#include "touch_port.h"
#include "sys_TPAdc.h"

char touch_init(void) //初始化触摸屏控制器
{
    return (char)F1C_TP_Init();
}

char touch_exit(void); //移除触摸屏控制器

uint8_t touch_scan(uint8_t i) //扫描触摸屏
{
    return F1C_TP_Scan(i);
}
