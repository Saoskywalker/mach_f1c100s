#include "sys_audio.h"
#include "MTF_io.h"
#include "io.h"
#include "delay.h"

#define AC_DAC_DPC		    0x00
#define AC_DAC_FIFOC	    0x04
#define AC_DAC_FIFOS	    0x08
#define AC_ADC_TXDATA	    0x0c
#define AC_ADC_FIFOC	    0x10
#define AC_ADC_FIFOS	    0x14
#define AC_ADC_RXDATA	    0x18
#define DAC_MIXER_CTRL	    0x20
#define ADC_MIXER_CTRL	    0x24
#define ADDA_TUNE    	      0x28
#define BIAS_DA16_CAL_CTRL0	0x2C
#define BIAS_DA16_CAL_CTRL1	0x34

static uint8_t sysAudioVol = 8; //音量大小

#define SampleRate48KHZ 	0//000: 48KHz
#define SampleRate24KHZ 	2//010: 24KHz
#define SampleRate12KHZ		4//100: 12KHz
#define SampleRate192KHZ	6//110: 192KHz
#define SampleRate32KHZ		1//001: 32KHz
#define SampleRate16KHZ		3//011: 16KHz
#define SampleRate8KHZ		5//101: 8KHz
#define SampleRate96KHZ		7//111: 96KHz
//
#define SampleRate44_1KHZ			0//000: 48KHz
#define SampleRate22_05KHZ		2//010: 24KHz
#define SampleRate11_025KHZ		4//100: 12KHz

#define snd_soc_update_bits(Base_Add,Offset,bit,Clear,Value)	write32(Base_Add+Offset,(read32(Base_Add+Offset) & (~((u64_t)(Clear)<<bit))) | ((u64_t)(Value)<<bit)) 
//#define snd_soc_update_bits(Base_Add,Offset,Clear,Value)	write32(Base_Add+Offset,read32(Base_Add+Offset) & (~((u64_t)(Clear))) | ((u64_t)(Value))    ) 

/////////////////////////////////////////////
/*DMA初始化*/
void DMA_Init(void)
{
static int Inif_f=0;
	if(Inif_f==0)//初始化一次
	{
		//使能DMA时钟
		write32(CCU_BUS_CLK_GATING_REG0,read32(CCU_BUS_CLK_GATING_REG0)|(1)<<6);
		//使能DMA复位
		write32(CCU_BUS_SOFT_RST_REG0,read32(CCU_BUS_SOFT_RST_REG0)|((1)<<6));
		delay_us(100);
		Inif_f=1;
	}
}

/*音频DMA参数初始化
dma_ch ：dma通道0-3
Source ：目标指针
bcnt	：计数 byte
*/
void SET_AUDIO_DMA_DATA(int dma_ch,unsigned int *Source,unsigned int bcnt)
{
int n=0;
//	printf("DMA Update...\r\n");
  n=dma_ch;
	
  //自动模式下【Auto Clock】一定要设置为0，不然两个时钟会传一个数据过去
  write32(DMA_Base_Address+0x8,read32(DMA_Base_Address+0x8) &(~ ((0x1)<<16)) );	
	write32(DMA_Base_Address+0x8,0);	
	
	//设置源地址
	write32(DMA_Base_Address+(0x100+n*0x20+0x4),(u32_t)Source);
	//设置目标地址
	write32(DMA_Base_Address+(0x100+n*0x20+0x8),(u32_t)(AUDIO_BASE+0x0C));
	//设置BCNT
	write32(DMA_Base_Address+(0x100+n*0x20+0xC),bcnt);

	//设置DMA连续使能
//	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<29));	

	//设置源类型 SDRAM memory
	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x11)<<0));
	//设置源宽度为16位  0-8 1-16 2-32
	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<8));
	//设置源地址模式 IO  0-线 1-IO
//	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<5));	
	//源 Burst Length 0-1 1-4
//	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<7));	
	
	//设置目标类型 AUDIO
	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0xc)<<16));	
	//设置目标宽度为16位  0-8 1-16 2-32
	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<24));		
	//设置目标地址模式 IO 0-线 1-IO
	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<21));		
	//目标 Burst Length0-1 1-4
// write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<23));	

	write32(DMA_Base_Address+(0x100+n*0x20+0x0),read32(DMA_Base_Address+(0x100+n*0x20+0x0))|((0x1)<<15));
			
	//设置DMA加载使能
	S_BIT(DMA_Base_Address+(0x100+n*0x20+0x0),31);
}

/*dma disable*/
void dmd_disable(int dma_ch)
{
	C_BIT(DMA_Base_Address+(0x100+dma_ch*0x20+0x0),31);
}

/*音频初始化*/
void AUDIO_Init(void)
{
static int af=0;
	if(af==0)
	{
	af=1;
		printf("AUDIO Init...\r\n");
		//使能总线AUDIO时钟
		write32(CCU_BUS_CLK_GATING_REG2,read32(CCU_BUS_CLK_GATING_REG2)|(1)<<0);
		//使能AUDIO复位
		write32(CCU_BUS_SOFT_RST_REG2,read32(CCU_BUS_SOFT_RST_REG2)|((1)<<0));
		delay_ms(1);

		
		//PLL使能
		S_BIT(PLL_AUDIO_CTRL_REG,31);
		//等PLL有效
		while((read32(PLL_AUDIO_CTRL_REG)&(1<<28))==0);
		
		#define DAC_DRQ_CLR_CNT			21
		#define FIFO_FLUSH					0
		#define FIR_VER							28
		#define ADC_FIFO_FLUSH			0
		#define PH_COM_FC        		22
		#define PH_COM_PROTEC      	21
		#define HP_VOL              0
		#define EN_DAC							31
		#define HP_POWER_EN					15	
		#define R_MIXER_MUTE_R_DAC	17
		#define R_MIXER_MUTE_L_DAC	16
		#define L_MIXER_MUTE_R_DAC	9
		#define L_MIXER_MUTE_L_DAC	8
		#define PH_R_MUTE						27  
		#define PH_L_MUTE						26  
		#define DAC_AG_R_EN					31  
		#define DAC_AG_L_EN					30  
		
		snd_soc_update_bits(codec, AC_DAC_FIFOC,DAC_DRQ_CLR_CNT,0x3,0x3);
		snd_soc_update_bits(codec, AC_DAC_FIFOC,FIFO_FLUSH,0x1,0x1);
		/*
		*	0:64-Tap FIR
		*	1:32-Tap FIR
		*/	
		snd_soc_update_bits(codec, AC_DAC_FIFOC,FIR_VER,0x1,0x0);	
		snd_soc_update_bits(codec, AC_ADC_FIFOC,ADC_FIFO_FLUSH,0x1,0x1);
		
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,PH_COM_PROTEC,0x1,0x1);
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,PH_COM_FC,0x3,0x3);	

		snd_soc_update_bits(codec, DAC_MIXER_CTRL,HP_VOL,0x3f,sysAudioVol|(~0X3F));/* set hp volume */
		snd_soc_update_bits(codec, AC_DAC_DPC,EN_DAC,0x1,0x1);
		
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,HP_POWER_EN,0x1,0x1);	/* power hp */
		
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,L_MIXER_MUTE_R_DAC,0x1,0x1);	
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,L_MIXER_MUTE_L_DAC,0x1,0x1);
		
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,R_MIXER_MUTE_R_DAC,0x1,0x1);	
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,R_MIXER_MUTE_L_DAC,0x1,0x1);	
		
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,PH_L_MUTE,0x1,0x1);	
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,PH_R_MUTE,0x1,0x1);	

		snd_soc_update_bits(codec, DAC_MIXER_CTRL,DAC_AG_L_EN,0x1,0x1);	
		snd_soc_update_bits(codec, DAC_MIXER_CTRL,DAC_AG_R_EN,0x1,0x1);	
		
		/*音频放大开关控制,需要先初始化，再开外部放大器，不然初始化时有杂音*/
		delay_ms(500);	
		// gpio_f1c100s_set_dir(&GPIO_PE, 9, GPIO_DIRECTION_OUTPUT);
		// gpio_f1c100s_set_value(&GPIO_PE, 9, 0);
		delay_ms(500);

		//DMA使用时要开空中断
		S_BIT(AUDIO_BASE+4,4);
		//发送最后采样
		S_BIT(AUDIO_BASE+4,26);
	}
}


/*播放初始化*/
int AUDIO_PLAY_Init(int sample_rate, uint16_t bits_per_sample, uint8_t num_channels)	
{
int s=0,n=0,m=0;	

	switch(sample_rate)
	{
		case 48000:
			s=SampleRate48KHZ;n=0x55;m=0x14;
			break;
		case 24000:
			s=SampleRate24KHZ;n=0x55;m=0x14;
			break;	
		case 12000:
			s=SampleRate12KHZ;n=0x55;m=0x14;
			break;
		case 192000:
			s=SampleRate192KHZ;n=0x55;m=0x14;
			break;	
		case 32000:
			s=SampleRate32KHZ;n=0x55;m=0x14;
			break;
		case 16000:
			s=SampleRate16KHZ;n=0x55;m=0x14;
			break;			
		case 8000:
			s=SampleRate8KHZ;n=0x55;m=0x14;
			break;
		case 96000:
			s=SampleRate96KHZ;n=0x55;m=0x14;
			break;	
		case 44100:
			s=SampleRate44_1KHZ;n=0x4E;m=0x14;
			break;			
		case 22050:
			s=SampleRate22_05KHZ;n=0x4E;m=0x14;
			break;
		case 11025:
			s=SampleRate11_025KHZ;n=0x4E;m=0x14;
			break;	
		default:
			return -1;
	}
	//PLL关
	C_BIT(PLL_AUDIO_CTRL_REG,31);
	
	//-设置PLL-N
	C_Vlue(PLL_AUDIO_CTRL_REG,8,0x7f);
	S_Vlue(PLL_AUDIO_CTRL_REG,8,n);
	//-设置PLL-M
	C_Vlue(PLL_AUDIO_CTRL_REG,0,0x1f);
	S_Vlue(PLL_AUDIO_CTRL_REG,0,m);

	//PLL使能
	S_BIT(PLL_AUDIO_CTRL_REG,31);
	//等PLL有效
	while((read32(PLL_AUDIO_CTRL_REG)&(1<<28))==0);
			
		
	/*设置采样周期*/
	C_Vlue(AUDIO_BASE+4,29,0xf);
	S_Vlue(AUDIO_BASE+4,29,s);
	
	
	/*设置采样位宽*/
	if(bits_per_sample==16)
		C_BIT(AUDIO_BASE+4,5); //16位
	else if(bits_per_sample==24)
		S_BIT(AUDIO_BASE+4,5); //24位
	else return -1;
	
	/*设置采样*/
	if(bits_per_sample==16)
	{
		/*设置采样低16位*/
		C_Vlue(AUDIO_BASE+4,24,0x3);
		S_Vlue(AUDIO_BASE+4,24,0x3);	
	}
	else if(bits_per_sample==24)
	{
		/*设置采样高24位*/
		C_Vlue(AUDIO_BASE+4,24,0x3);
	}
	else return -1;
	
	/*设置为单声道*/
	if(num_channels==1)
		S_BIT(AUDIO_BASE+4,6);//单声道
	else if(num_channels==2)
		C_BIT(AUDIO_BASE+4,6);//双声道
	else return -1;
	
	//清采样计数
	write32(AUDIO_BASE+0x40,0);
	
return 0;
}

void AUDIO_VOL(unsigned char i)
{
	sysAudioVol = i;
	snd_soc_update_bits(codec, DAC_MIXER_CTRL,HP_VOL,0x3f,sysAudioVol|(~0X3F)); //F1C100S音量0~63
}
