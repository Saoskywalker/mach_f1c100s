#ifndef _SYS_AUDIO_H
#define _SYS_AUDIO_H

#include "types_base.h"
#include "types.h"

/*DMA-寄存器地址*/
#define DMA_Base_Address (0x01C02000)
/*寄存器地址*/
#define F1C100S_AUDIO_BASE  (0x01C23C00)
#define AUDIO_BASE    F1C100S_AUDIO_BASE
#define codec         F1C100S_AUDIO_BASE 
//
#define CCU_Base_Address     	  		(u32_t)0x01C20000
#define CCU_BUS_CLK_GATING_REG0 		(u32_t)CCU_Base_Address+0x0060
#define CCU_BUS_CLK_GATING_REG1 		(u32_t)CCU_Base_Address+0x0064
#define CCU_BUS_CLK_GATING_REG2 		(u32_t)CCU_Base_Address+0x0068
#define CCU_BUS_SOFT_RST_REG0 			(u32_t)CCU_Base_Address+0x02C0
#define CCU_BUS_SOFT_RST_REG1 			(u32_t)CCU_Base_Address+0x02C4
#define CCU_BUS_SOFT_RST_REG2 			(u32_t)CCU_Base_Address+0x02D0

//AUDIO CLK
#define PLL_AUDIO_CTRL_REG 						(u32_t)CCU_Base_Address+0x0008
#define AUDIO_CODEC_CLK_REG						(u32_t)CCU_Base_Address+0x0140
#define PLL_AUDIO_BIAS_REG						(u32_t)CCU_Base_Address+0x0224
#define PLL_AUDIO_PAT_CTRL_REG  	    (u32_t)CCU_Base_Address+0x0284

void DMA_Init(void);
void SET_AUDIO_DMA_DATA(int dma_ch,unsigned int *Source,unsigned int bcnt);
void dmd_disable(int dma_ch);
void AUDIO_Init(void);
int AUDIO_PLAY_Init(int sample_rate, uint16_t bits_per_sample, uint8_t num_channels);
void AUDIO_VOL(unsigned char i);

#endif
