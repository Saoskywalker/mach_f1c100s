#ifndef _FRAMEBUFFER_PORT_H
#define _FRAMEBUFFER_PORT_H

#include "types_base.h"

// struct fb_colorfield
// {
//     /*fb缓存的RGB位域，该结构描述每一个像素显示缓冲区的组织方式，假如为RGB565模式，R占5位=bit[11:15]
//                                        G占6位=bit[10:5] B占5位=bit[4:0] */
//     uint32_t offset; /* beginning of bitfield */             /*位域偏移:red=11 ,green=5 blue=0 */
//     uint32_t length; /* length of bitfield  */               /*位域长度 red=5 green=6 blue=5 */
//     uint32_t msb_right; /* != 0 : Most significant bit is */ /*msb_right!=0=>MSB在右边 */
// };

typedef struct
{
    //struct fb_info的成员（可变参数），其记录用户可修改的显示控制器的参数，包括分
    //辨率和每个像素点的比特数，其成员需要在驱动程序中初始化和设置

    /********可见解析度（实际屏幕）********/
    uint32_t xres; /* visible resolution*/ /*定义屏幕一行有多少个像素点 */
    uint32_t yres;                         /*定义屏幕一列由多少个像素点 */

    /********虚拟解析度（虚拟屏幕）********/
    uint32_t xres_virtual; /* virtual resolution*/         /*虚拟屏幕一行有多少个像素点 */
    uint32_t yres_virtual;                                 /*虚拟屏幕一列由多少个像素点*/
    uint32_t xoffset; /* offset from virtual to visible */ /*虚拟到可见（实际）之间的行方向偏移 */
    uint32_t yoffset; /* resolution*/                      /*虚拟到可见（实际）之间的列方向偏移*/

    uint32_t bits_per_pixel; /* guess what */                   /*每像素位数(多少BPP) */
    uint32_t grayscale; /* != 0 Graylevels instead of colors */ /*非0时指灰度*/

    /********fb缓存的RGB位域**********/
    // struct fb_colorfield red; /* bitfield in fb mem if true color, */ /* fb缓存的红色位域*/
    // struct fb_colorfield green; /* else only length is significant */ /* fb缓存的绿色位域*/
    // struct fb_colorfield blue;                                        /* fb缓存的蓝色位域*/
    // struct fb_colorfield transp; /* transparency*/                    /*透明度 =0 */

    // uint32_t nonstd; /* != 0 Non standard pixel format */ /*非标准像素格式时应该为非0值 (标志像素格式时 nonstd=0) */

    uint32_t activate; /* see FB_ACTIVATE_**/ /*查看宏FB_ACTIVATE_NOW */

    uint32_t height; /* height of picture in mm */ /*屏幕高度(毫米)*/
    uint32_t width; /* width of picture in mm*/    /*屏幕宽度(毫米)*/

    uint32_t accel_flags; /* (OBSOLETE) see fb_info.flags */ /*查看fb_info.flags */

    /************这参数必须通过查看LCD数据手册得到**************/
    /* Timing: All values in pixclocks, except pixclock (of course) */
    uint32_t pixclock;

    /* 行切换，从同步到绘图之间的延迟即HFPD(有效数据之后无效的像素的个数) ，对应于LCD数据手册的Hsyn的front-porch*/
    uint32_t left_margin; /* time from sync to picture */
    /*行切换，从绘图到同步之间的延迟即HBPD(Hsyn脉冲下降沿之后的无效像素的个数) ,对应于LCD数据手册的Hsyn的back-porch*/
    uint32_t right_margin; /* time from picture to sync */
    /*帧切换，从同步到绘图之间的延迟即VFPD(有效数据之后还要经历的无效行数(之后是下一帧数据)) ，对应于LCD数据手册的Vsyn的front-porch*/
    uint32_t upper_margin; /* time from sync to picture */
    /*帧切换，从绘图到同步之间的延迟即VBPD(Vsyn脉冲下降沿之后还要经历的无效行数) ，对应于LCD数据手册的Vsyn的back-porch */
    uint32_t lower_margin;
    /*水平同步的长度即HSPW(Hsyn信号的脉冲宽度)，对应于LCD数据手册的Hsyn的pulse Width */
    uint32_t hsync_len; /* length of horizontal sync*/
    /*垂直同步的长度即VSPW(Vsyn信号的脉冲宽度)，对应于LCD数据手册的Vsyn的pulse Width */
    uint32_t vsync_len; /* length of vertical sync*/

    uint32_t sync; /* see FB_SYNC_**/                        /* 同步时钟模式*/
    uint32_t vmode; /* see FB_VMODE_**/                      
    uint32_t rotate; /* angle we rotate counter clockwise */ /*逆时钟旋转的角度 */
    // uint32_t reserved[5]; /* Reserved for future compatibility */

    uint32_t scale_flag; /*硬件缩放标志*/
    uint32_t tv_input_flag; /*是否使用cvbs输入*/
    void *write_back_addr; /*回写显存地址*/
} framebuffer_dev; //显示器的硬件信息, 及硬件控制

typedef enum 
{
	PIXEL_FORMAT_ARGB32		= 0,
	PIXEL_FORMAT_RGB24		= 1,
	PIXEL_FORMAT_A8			= 2,
	PIXEL_FORMAT_A1			= 3,
	PIXEL_FORMAT_RGB16_565	= 4,
	PIXEL_FORMAT_RGB30		= 5,
}pixel_format_t;

typedef struct {
	/* The width of render */
	uint32_t width;

	/* The height of render */
	uint32_t height;

	/* Pixel format */
	pixel_format_t format;

	/* Pixel data */
	void * pixels;   
	
	/* full render data length(unit: byte) */
	size_t pixlen;

    /*how many byte of every pixel*/
    uint32_t bytes_per_pixel;

	/* Private data */
	void * priv;
}render_dev_type; //渲染设备, 应用程序读写显示的矩阵数据

//截屏功能
void MTF_fb_write_back_start(void); //截屏一帧  
uint8_t MTF_fb_write_back_state(void); //查询完成状态

render_dev_type *MTF_fb_render_create(framebuffer_dev *fb, uint32_t width, uint32_t height);
void MTF_fb_destroy(render_dev_type *render);
void MTF_fb_present(framebuffer_dev *fb, render_dev_type *render); //更新显示一帧
void *MTF_fb_get_dis_mem(render_dev_type *render); //获取可直接使用的显存地址

void MTF_fb_set_backlight(framebuffer_dev *fb, int32_t brightness); //设置背光  
void MTF_fb_TV_input(framebuffer_dev *fb, uint8_t state, uint8_t contrast, uint8_t bright, uint8_t saturation, uint8_t hue); //是否开启AV输入
void MTF_fb_scale(framebuffer_dev *fb, uint8_t state, void *data); //是否开启硬件缩放

void MTF_fb_init(framebuffer_dev *fb);
void MTF_fb_exit(framebuffer_dev *fb);

#endif
