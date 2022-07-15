#include "touch_port.h"
#include "MTF_io.h"
#include "sys_TPAdc.h"
#include "touch.h"
#include "ts_calibrate.h"

#define DEBUG_TOUCH(...) //printf(__VA_ARGS__)

uint8_t touch_init(void) //初始化触摸屏控制器
{
    tp_dev.touchtype &= ~0X80; //设为电阻屏
    return F1C_TP_Init();
}

uint8_t touch_exit(void) //移除触摸屏控制器
{
    return 0;
}

uint8_t touch_scan(int *x, int *y, uint8_t target_num, uint8_t *result_num) //扫描触摸屏
{
    int x_get = 0, y_get = 0, xy_temp = 0, count = 0;
    int x_out = 0;
    int y_out = 0;
    int temp = 0;

    if (F1C_TP_Scan(&x_get, &y_get))
    {
        /******按下*******/
        if (tp_dev.touchtype & 0x01) //x, y轴需交换
        {
            xy_temp = x_get;
            x_get = y_get;
            y_get = xy_temp;
        }
        if (x_get == 0 || y_get == 0) //有时错误读出的是0
        {
            if (++count >= 5)
                count = 5;
            else
                return 0;
        }
        else
        {
            count = 0;
        }

        if (TPAjustFlag)
        { //读取物理坐标
            x[0] = x_get;
            y[0] = y_get;
            DEBUG_TOUCH("dev: x=%d y=%d \r\n", x[0], y[0]);
        }
        else //读取屏幕坐标
        {
            //将结果转换为屏幕坐标
            coords_get(&touch_cal, &x_get, &y_get, &x_out, &y_out); //五点校准
            if (x_out < tp_dev.width && x_out >= 0 && y_out < tp_dev.height && y_out >= 0) //防止超范围
            {
                x[0] = x_out;
                y[0] = y_out;
                // x[0] = tp_dev.xfac * x + tp_dev.xoff; //四点校准
                // y[0] = tp_dev.yfac * y + tp_dev.yoff;

                //方向转换
                if (((tp_dev.touchtype & 0x06) >> 1) == 0) // 90度
                {
                    temp = y[0];
                    y[0] = x[0];
                    x[0] = tp_dev.width - 1 - temp;
                }
                else if (((tp_dev.touchtype & 0x06) >> 1) == 2) // 270度
                {
                    temp = x[0];
                    x[0] = y[0];
                    y[0] = tp_dev.height - 1 - temp;
                }
                else if (((tp_dev.touchtype & 0x06) >> 1) == 3) // 180度
                {
                    x[0] = tp_dev.width - 1 - x[0];
                    y[0] = tp_dev.height - 1 - y[0];
                }
                else // 0度
                {
                }
                DEBUG_TOUCH("dis: %d, %d\r\n", x[0], y[0]);
            }
        }
        if (*result_num == 0) //之前没有被按下
        {
            (*result_num)++; //一个按键按下
        }
    }
    else
    {
        /******松开********/
        count = 0;
        if (*result_num) //之前是被按下的
        {
            *result_num = 0; //标记按键松开
        }
    }

    return 0;
}
