#include "framebuffer_port.h"
#include "framebuffer.h"
#include "LCD_parameter.h"
#include "malloc.h"
#include "sys_tvd.h"
#include "sys_defe.h"
#include "delay.h"
#include <dma.h>

//目前F1C100S配置威1级缓冲
#define DIS_BUF_LEVEL 1 //显示缓存级数: 1~3
static char _use_vram_flag = 0;

//截屏功能
void MTF_fb_write_back_start(void) //截屏一帧 
{
    fb_f1c100s_write_back_start();
}

uint8_t MTF_fb_write_back_state(void) //查询完成状态
{
    uint32_t i = 0, len = fb_f1c100s_pData.width * fb_f1c100s_pData.height;
    uint32_t *data = fb_f1c100s_pData.write_back_ram;

    delay_ms(200); // wait

    //f1c100s need RGB_BR_swap
    for (i = 0; i < len; i++)
    {
        data[i] = ((data[i] & 0X00FF0000) >> 16) |
                  (data[i] & 0XFF00FF00) | ((data[i] & 0X000000FF) << 16);
    }

    return 0;

    // return fb_f1c100s_write_back_state();
}

void MTF_fb_set_backlight(framebuffer_dev *fb, int32_t brightness) //设置背光
{
    fb_f1c100s_setbl(&fb_f1c100s_pData, brightness); //设置背光
}

render_dev *MTF_fb_render_create(framebuffer_dev *fb, uint32_t width, uint32_t height)
{
    
	render_dev * render;
	void * pixels;
	size_t pixlen = width * height * fb_f1c100s_pData.bytes_per_pixel;

    render = malloc(sizeof(render_dev));
    if (render == NULL)
        return NULL;

#if DIS_BUF_LEVEL == 1
    static char i = 0;
    if (pixlen == fb_f1c100s_pData.width * fb_f1c100s_pData.height * fb_f1c100s_pData.bytes_per_pixel)
    {
        _use_vram_flag = 1;
        if (i) //直接写显存
        {
            i = 0;
            pixels = fb_f1c100s_pData.vram[(fb_f1c100s_pData.index + 1) & 0x1];
        }
        else
        {
            i = 1;
            pixels = fb_f1c100s_pData.vram[fb_f1c100s_pData.index];
        }
    }
    else
#endif
    {
        //不直接写显存, 通过特定函数将其资料复制至显存
        pixels = dma_alloc_coherent(pixlen);
        if (pixels == NULL)
        {
            free(render);
            return NULL;
        }
    }

    render->width = fb_f1c100s_pData.width;
	render->height = fb_f1c100s_pData.height;
	render->format = PIXEL_FORMAT_ARGB32;
	render->pixels = pixels;
	render->pixlen = pixlen;
    render->bytes_per_pixel = fb_f1c100s_pData.bytes_per_pixel;
	render->priv = NULL;

	return render;
}

void MTF_fb_destroy(render_dev * render)
{
	if(render)
	{
#if DIS_BUF_LEVEL == 1
        if (render->pixels != fb_f1c100s_pData.vram[0] || render->pixels != fb_f1c100s_pData.vram[1])
            dma_free_coherent(render->pixels);
#else
        dma_free_coherent(render->pixels);
#endif
        free(render);
    }
}

void MTF_fb_present(framebuffer_dev *fb, render_dev * render)
{
#if DIS_BUF_LEVEL == 3
    fb_f1c100s_present(&fb_f1c100s_pData, render->pixels, render->pixlen); //三级缓冲, 缩放显示时不能用使用此方法
#endif
}

void *MTF_fb_get_dis_mem(render_dev *render) //获取可直接使用的显存地址
{
    return render->pixels;
}

void MTF_fb_TV_input(framebuffer_dev *fb, uint8_t state, uint8_t contrast, uint8_t bright, uint8_t saturation, uint8_t hue) //是否开启AV输入
{
//当f1c100s开启AV输入时, 调用fb_f1c100s_change_hw会造成配置错误(无法显示), 
//当f1c100s开启TV时禁止再调用fb_f1c100s_change_hw

    static uint8_t av_in = 0;

    if (state) //开启AV输入
    {
        if (av_in == 0)
            fb_f1c100s_change_hw(1, 1);
        av_in = 1;
        f1c100s_tvd_AutoMode(); //自动模式
    }
    else //关闭av输入
    {
        TVD_Remove();
        if (fb->scale_flag)
        {
            if (av_in == 0)
                fb_f1c100s_change_hw(0, 1);
            Defe_Disply_scale(fb->xres_virtual, fb->yres_virtual); //AV显示通道让回显示
        }
        else
        {
            if (av_in == 0)
                fb_f1c100s_change_hw(1, 0);
        }     
    }

    if (state)
    {
        com_video_set_contrast(contrast);   //调整对比度 默认128
        com_video_set_bright(bright);     //调整亮度 默认32
        com_video_set_saturation(saturation); //调整饱和度 默认128
        com_video_set_hue(hue);        //调整色调 默认0
    }

    fb->tv_input_flag = state;
}

void MTF_fb_scale(framebuffer_dev *fb, uint8_t state, void *data) //是否开启硬件缩放
{
    //f1c100s缩放由硬件运算后直接显示, 和cvbs同层, 两者不能同时使用
	defe_buf_addr = (u32 *)data;
    fb_f1c100s_pData.layer1_enable = 0; //关闭layer1
	fb_f1c100s_change_hw(0, 1); //缩放后默认BR换转,,手动换回,并开启Layer0到VIDEO
	Defe_Disply_scale(fb->xres_virtual, fb->yres_virtual);
    // int _icc = 0;
	// for (_icc = 0; _icc < render->width*render->height; _icc++) //上层透明, 显示下层(硬件缩放层)
	// 	((ColorClass *)render->pixels)[_icc] = 0;
	delay_ms(45);
	fb_f1c100s_flush_cache_set(defe_buf_addr);
    fb->scale_flag = 1; //开启缩放
}

void MTF_fb_init(framebuffer_dev *fb)
{   
    //获取液晶参数
    fb->xres = LCD_X_PIXEL;
    fb->yres = LCD_Y_PIXEL;
    fb->xres_virtual = fb->xres;
    fb->yres_virtual = fb->yres;
    fb->sync = LCD_DE_HV_MODE;               // 0: DE_HV 1:DE 2:HV
    fb->pixclock = LCD_CLK;                  // pixel_clock_hz
    fb->left_margin = LCD_HFP;               // h_front_porch
    fb->right_margin = LCD_HBP;              // h_back_porch
    fb->hsync_len = LCD_HSL;                 // h_sync_len
    fb->upper_margin = LCD_VFP;              // v_front_porch
    fb->lower_margin = LCD_VBP;              // v_back_porch
    fb->vsync_len = LCD_VSL;                 // v_sync_len
    fb->width = 216;                         //液晶尺寸(单位:mm)
    fb->height = 135;
    fb_f1c100s_pData.backlight_max = LCD_BACKLIGHT_MAX; // 0~100

    //像素信息
    fb_f1c100s_pData.width = fb->xres;
    fb_f1c100s_pData.height = fb->yres;
    fb_f1c100s_pData.user_width = fb->xres_virtual;
    fb_f1c100s_pData.user_height = fb->yres_virtual;
    fb_f1c100s_pData.pwidth = fb->width;
    fb_f1c100s_pData.pheight = fb->height;
    fb->bits_per_pixel = 18;
    fb_f1c100s_pData.bits_per_pixel = fb->bits_per_pixel;
    fb_f1c100s_pData.bytes_per_pixel = 4;

    //LCD信号时序
    fb_f1c100s_pData.timing.pixel_clock_hz = fb->pixclock;
    fb_f1c100s_pData.timing.h_front_porch = fb->left_margin;
    fb_f1c100s_pData.timing.h_back_porch = fb->right_margin;
    fb_f1c100s_pData.timing.v_front_porch = fb->upper_margin;
    fb_f1c100s_pData.timing.v_back_porch = fb->lower_margin;
    fb_f1c100s_pData.timing.h_sync_len = fb->hsync_len;
    fb_f1c100s_pData.timing.v_sync_len = fb->vsync_len;
    fb_f1c100s_pData.timing.DE_HV_mode = fb->sync; //同步时钟模式设置 0: DE_HV 1:DE 2:HV

    fb_f1c100s_pData.timing.h_sync_active = 0; //时钟信号是否翻转
    fb_f1c100s_pData.timing.v_sync_active = 0;
    fb_f1c100s_pData.timing.den_active = 1;
    fb_f1c100s_pData.timing.clk_active = 0;

    fb->scale_flag = 0; //不开启缩放
    fb->tv_input_flag = 0; //不使用cvbs输入
    fb->write_back_addr = fb_f1c100s_pData.write_back_ram; //回写显存地址

    fb_f1c100s_init(&fb_f1c100s_pData);
}

void MTF_fb_exit(framebuffer_dev *fb)
{

}
