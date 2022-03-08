#include "audio_port.h"
#include "sys_audio.h"
#include "types_base.h"
#include "io.h"
#include "timer_f1c100s.h"

static unsigned char _pcmFristOutput = 0;
static uint32_t pcm_run_time = 0;

void MTF_audio_time(void)
{
	pcm_run_time++; //每10ms一次
}

uint32_t MTF_audio_output_time_get(void)
{
	// return AVS_Time_Read(AVS_TIME_0) / 100; //硬件方式计时
	return pcm_run_time; //软件方式计时
}

char MTF_audio_pcm_output_init(audio_pcm_dev *dev, audio_pcm_dev *dest)
{
	pcm_run_time = 0;
	_pcmFristOutput = 0;
	AUDIO_Init();
	DMA_Init();
	AVS_Time_Init();
	AUDIO_PLAY_Init(dev->freq, dev->format, dev->channels);
	return 0;
}

char MTF_audio_pcm_output_exit(audio_pcm_dev *dev)
{
	pcm_run_time = 0;
	_pcmFristOutput = 0;
	S_BIT(DMA_Base_Address + 0x4, 1);
	dmd_disable(0); //停止DMA
	AVS_Time_Stop(AVS_TIME_0); //停止计时器
	C_BIT(AUDIO_CODEC_CLK_REG,31);
	return 0;
}

uint8_t MTF_audio_pcm_output_busy(audio_pcm_dev *dev)
{
	return !(read32(DMA_Base_Address+0x4)&0x2);
}

uint8_t MTF_audio_pcm_output(audio_pcm_dev *dev, uint8_t *stream, int len)
{
	if (_pcmFristOutput) //后续导入播放PCM
	{
		S_BIT(DMA_Base_Address + 0x4, 1); //清标记
		dmd_disable(0);
		SET_AUDIO_DMA_DATA(0, (unsigned int *)stream, len); //通过DMA导入至PCM
	}
	else //第一次导入播放PCM
	{
		_pcmFristOutput = 1;
		SET_AUDIO_DMA_DATA(0, (unsigned int *)stream, len);
		//时钟选通
		S_BIT(AUDIO_CODEC_CLK_REG, 31);
		//开始计时器
		AVS_Time_Start(AVS_TIME_0);
	}
	return 0;
}

void MTF_audio_vol(unsigned char i)
{
	AUDIO_VOL(i);
}
