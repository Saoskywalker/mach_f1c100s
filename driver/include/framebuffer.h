#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <types.h>
#include <pwm-f1c100s.h>

typedef struct
{
	virtual_addr_t virtdefe;
	virtual_addr_t virtdebe;
	virtual_addr_t virttcon;

	char * clkdefe;
	char * clkdebe;
	char * clktcon;
	int32_t rstdefe;
	int32_t rstdebe;
	int32_t rsttcon;
	int32_t width;
	int32_t height;
	int32_t user_width; /* display ram width and height, mainly use for scale display */
	int32_t user_height;
	int32_t pwidth;
	int32_t pheight;
	int32_t bits_per_pixel;
	int32_t bytes_per_pixel;
	int32_t index;
	void * vram[2];
	void * write_back_ram;
	char swapBR; /* color BR swap */
	char layer0_enable;
	char layer1_enable;
	void * cleanCacheAddr;

	struct
	{
		int32_t pixel_clock_hz;
		int32_t h_front_porch;
		int32_t h_back_porch;
		int32_t h_sync_len;
		int32_t v_front_porch;
		int32_t v_back_porch;
		int32_t v_sync_len;
		int32_t h_sync_active;
		int32_t v_sync_active;
		int32_t den_active;
		int32_t clk_active;
		int32_t DE_HV_mode;
	} timing;

	pwm_t * backlight;
	int32_t brightness;
	int32_t backlight_max;
}fb_f1c100s_pdata_t;

extern fb_f1c100s_pdata_t fb_f1c100s_pData;

void fb_f1c100s_write_back_start(void);
uint8_t fb_f1c100s_write_back_state(void);
void framebuffer_flush(void);
void fb_f1c100s_setbl(fb_f1c100s_pdata_t * pdat, int32_t brightness);
int32_t fb_f1c100s_getbl(fb_f1c100s_pdata_t * pdat);
void fb_f1c100s_change_hw(char BR, char layer0);
void fb_f1c100s_flush_cache_set(void *addr);

void fb_f1c100s_present(fb_f1c100s_pdata_t *pdat, void *pixels, size_t pixlen);

void fb_f1c100s_init(fb_f1c100s_pdata_t * pdat);

void fb_f1c100s_remove(fb_f1c100s_pdata_t * pdat);

void fb_f1c100s_suspend(fb_f1c100s_pdata_t * pdat);

void fb_f1c100s_resume(fb_f1c100s_pdata_t * pdat);


#ifdef __cplusplus
}
#endif

#endif /* __FRAMEBUFFER_H__ */
