#include "types_plus.h"
#include "jpeg.h"
#include "MTF_io.h"
#include "file_operate_hal.h"
#include "lcd.h"
#include "delay.h"
#include "dma_pool.h"
#include "Sagittarius_timer.h"

/* SYS */
typedef struct {
  __IO  u32 CTRL[16];       // 0x00 SRAM Control registers
} SYS_T;
#define SYS ((SYS_T*)0x01C00000)

/* CCU */
typedef struct {
  __IO  u32 PLL_CPU_CTRL;   // 0x00 PLL_CPU Control Register
  __IO  u32 RES0;
  __IO  u32 PLL_AUDIO_CTRL; // 0x08 PLL_AUDIO Control Register
  __IO  u32 RES1;
  __IO  u32 PLL_VIDEO_CTRL; // 0x10 PLL_VIDEO Control Register
  __IO  u32 RES2;
  __IO  u32 PLL_VE_CTRL;    // 0x18 PLL_VE Control Register
  __IO  u32 RES3;
  __IO  u32 PLL_DDR_CTRL;   // 0x20 PLL_DDR Control Register
  __IO  u32 RES4;
  __IO  u32 PLL_PERIPH_CTRL;// 0x28 PLL_PERIPH Control Register
  __IO  u32 RES5[9];
  __IO  u32 CPU_CLK_SRC;    // 0x50 CPU Clock Source Register
  __IO  u32 AHB_APB_CFG;    // 0x54 AHB/APB/HCLKC Configuration Register
  __IO  u32 RES6[2];
  __IO  u32 BUS_CLK_GATING0;// 0x60 Bus Clock Gating Register 0
  __IO  u32 BUS_CLK_GATING1;// 0x64 Bus Clock Gating Register 1
  __IO  u32 BUS_CLK_GATING2;// 0x68 Bus Clock Gating Register 2
  __IO  u32 RES7[7];
  __IO  u32 SDMMC0_CLK;     // 0x88 SDMMC0 Clock Register
  __IO  u32 SDMMC1_CLK;     // 0x8C SDMMC0 Clock Register
  __IO  u32 RES8[8];
  __IO  u32 DAUDIO_CLK;     // 0xB0 DAUDIO Clock Register
  __IO  u32 OWA_CLK;        // 0xB4 OWA Clock Register
  __IO  u32 CIR_CLK;        // 0xB8 CIR Clock Register
  __IO  u32 RES9[4];
  __IO  u32 USBPHY_CFG;     // 0xCC USBPHY Clock Register
  __IO  u32 RES10[12];
  __IO  u32 DRAM_GATING;    // 0x100 DRAM GATING Register
  __IO  u32 BE_CLK;         // 0x104 BE Clock Register
  __IO  u32 RES11;
  __IO  u32 FE_CLK;         // 0x10C FE Clock Register
  __IO  u32 RES12[2];
  __IO  u32 TCON_CLK;       // 0x118 TCON Clock Register
  __IO  u32 DI_CLK;         // 0x11C De-interlacer Clock Register
  __IO  u32 TVE_CLK;        // 0x120 TVE Clock Register
  __IO  u32 TVD_CLK;        // 0x124 TVD Clock Register
  __IO  u32 RES13[3];
  __IO  u32 CSI_CLK;        // 0x134 CSI Clock Register
  __IO  u32 RES14;
  __IO  u32 VE_CLK;         // 0x13C VE Clock Register
  __IO  u32 AC_DIG_CLK;     // 0x140 Audio Codec Clock Register
  __IO  u32 AVS_CLK;        // 0x144 AVS Clock Register
  __IO  u32 RES15[46];
  __IO  u32 PLL_STABLE0;    // 0x200 PLL stable time 0
  __IO  u32 PLL_STABLE1;    // 0x204 PLL stable time 1
  __IO  u32 RES16[6];
  __IO  u32 PLL_CPU_BIAS;   // 0x220 PLL CPU Bias Register
  __IO  u32 PLL_AUDIO_BIAS; // 0x224 PLL Audio Bias Register
  __IO  u32 PLL_VIDEO_BIAS; // 0x228 PLL Video Bias Register
  __IO  u32 PLL_VE_BIAS;    // 0x22c PLL VE Bias Register
  __IO  u32 PLL_DDR_BIAS;   // 0x230 PLL DDR Bias Register
  __IO  u32 PLL_PERIPH_BIAS;// 0x234 PLL PERIPH Bias Register
  __IO  u32 RES17[6];
  __IO  u32 PLL_CPU_TUNING; // 0x250 PLL CPU Tuning Register
  __IO  u32 RES18[3];
  __IO  u32 PLL_DDR_TUNING; // 0x260 PLL DDR Tuning Register
  __IO  u32 RES19[8];
  __IO  u32 PLL_AUDIO_PAT;  // 0x284 PLL Audio Pattern Control Register
  __IO  u32 PLL_VIDEO_PAT;  // 0x288 PLL Video Pattern Control Register
  __IO  u32 RES20;
  __IO  u32 PLL_DDR_PAT;    // 0x290 PLL DDR Pattern Control Register
  __IO  u32 RES21[11];
  __IO  u32 BUS_SOFT_RST0;  // 0x2C0 Bus Software Reset Register 0
  __IO  u32 BUS_SOFT_RST1;  // 0x2C4 Bus Software Reset Register 1
  __IO  u32 RES22[2];
  __IO  u32 BUS_SOFT_RST2;  // 0x2D0 Bus Software Reset Register 2
} CCU_T;
#define CCU ((CCU_T*)0x01C20000)

/* VE */
typedef struct {
  __IO  u32 CTRL;           // 0x00 Sub-Engine & RAM Type Select
  __IO  u32 RST;            // 0x04 Sub-Engines Reset
  __IO  u32 RES[58];
  __IO  u32 VERSION;        // 0xF0 IP Version register
} VE_GEN_T;
#define VE  ((VE_GEN_T*)0x01C0E000)

typedef struct {
  __IO  u32 PHDR;           // 0x00 MPEG12 Picture Header register
  __IO  u32 VOPHDR;         // 0x04 MPEG Video Object Plane Header
  __IO  u32 SIZE;           // 0x08 Frame size in MPEG macroblocks (16x16)
  __IO  u32 FRAME_SIZE;     // 0x0C Frame size in pixels
  __IO  u32 MBA;            // 0x10 MPEG Macro Block Address register
  __IO  u32 CTRL;           // 0x14 MPEG Control Register
  __IO  u32 TRIG;           // 0x18 MPEG Decoding Trigger
  __IO  u32 STATUS;         // 0x1C MACC MPEG Status register
  __IO  u32 TRBTRD_FIELD;   // 0x20 Temporal References (TRB(B-VOP)&TRD)
  __IO  u32 TRBTRD_FRAME;   // 0x24
  __IO  u32 VLD_ADDR;       // 0x28 MPEG Variable Length Decoding Address
  __IO  u32 VLD_OFFSET;     // 0x2C MPEG Variable Length Decoding Offset
  __IO  u32 VLD_LEN;        // 0x30 MPEG Variable Length Decoding Length
  __IO  u32 VLD_END;        // 0x34 MPEG video source buffer end
  __IO  u32 MBH_ADDR;       // 0x38 MBH buffer address
  __IO  u32 DCAC_ADDR;      // 0x3C DCAC Buffer address
  __IO  u32 BLK_OFFSET;     // 0x40 MPEG Block address???
  __IO  u32 NCF_ADDR;       // 0x44 NFC buffer address
  __IO  u32 REC_LUMA;       // 0x48 MPEG Luma reconstruct buffer
  __IO  u32 REC_CHROMA;     // 0x4C MPEG Chroma reconstruct buffer
  __IO  u32 FWD_LUMA;       // 0x50 MPEG Luma forward buffer
  __IO  u32 FWD_CHROMA;     // 0x54 MPEG Chroma forward buffer
  __IO  u32 BACK_LUMA;      // 0x58 MPEG Luma Back buffer
  __IO  u32 BACK_CHROMA;    // 0x5C MPEG Chroma Back buffer
  __IO  u32 RES0[8];
  __IO  u32 IQ_MIN_INPUT;   // 0x80 MPEG Inverse Quantization min input level
  __IO  u32 QP_INPUT;       // 0x84 MPEG Inverse Quantization input level
  __IO  u32 RES1[12];
  __IO  u32 JPEG_SIZE;      // 0xB8 JPEG Size
  __IO  u32 JPEG_MCU;       // 0xBC JPEG Minimum Coded Unit
  __IO  u32 JPEG_RES_INT;   // 0xC0 JPEG Restart Interval
  __IO  u32 RES2[2];
  __IO  u32 ROT_LUMA;       // 0xCC MPEG Rotate-Scale Luma buffer
  __IO  u32 ROT_CHROMA;     // 0xD0 MPEG Rotate-Scale Chroma buffer
  __IO  u32 ROT_SCALE_CTRL; // 0xD4 Control Rotate/Scale Buffe
  __IO  u32 JPEG_MCU_START; // 0xD8 JPEG Macro Cell Unit Start
  __IO  u32 JPEG_MCU_END;   // 0xDC JPEG Macro Cell Unit End
  __IO  u32 RAM_WRITE_PTR;  // 0xE0 Auto incremental pointer for rd/wr VE SRAM
  __IO  u32 RAM_WRITE_DATA; // 0xE4 FIFO Like Data register for wr/rd VE SRAM
} MPEG_T;
#define MPEG ((MPEG_T*)0x01C0E100)

struct FRAME {
  u16 *addr;
  int width;
  int height;
  int cx;
  int cy;
} *fr[2];

/******************************************************************************/
/*                                   JPG                                      */
/******************************************************************************/
void set_quantization_tables (struct jpeg_t *jpeg)
{
	int i;
	for (i = 0; i < 64; i++)
    MPEG->IQ_MIN_INPUT = (u32)(64 + i) << 8 | jpeg->quant[jpeg->comp[0].qt]->coeff[i];
	for (i = 0; i < 64; i++)
    MPEG->IQ_MIN_INPUT = (u32)(i) << 8 | jpeg->quant[jpeg->comp[1].qt]->coeff[i];
}

void set_huffman_tables(struct jpeg_t *jpeg)
{
	u32 buffer[512];
	memset(buffer, 0, 4*512);
	int i;
	for (i = 0; i < 4; i++)
	{
		if (jpeg->huffman[i])
		{
			int j, sum, last;

			last = 0;
			sum = 0;
			for (j = 0; j < 16; j++)
			{
				((uint8_t *)buffer)[i * 64 + 32 + j] = sum;
				sum += jpeg->huffman[i]->num[j];
				if (jpeg->huffman[i]->num[j] != 0)
					last = j;
			}
			memcpy(&(buffer[256 + 64 * i]), jpeg->huffman[i]->codes, sum);
			sum = 0;
			for (j = 0; j <= last; j++)
			{
				((uint16_t *)buffer)[i * 32 + j] = sum;
				sum += jpeg->huffman[i]->num[j];
				sum *= 2;
			}
			for (j = last + 1; j < 16; j++)
			{
				((uint16_t *)buffer)[i * 32 + j] = 0xffff;
			}
		}
	}
	for (i = 0; i < 512; i++) MPEG->RAM_WRITE_DATA = buffer[i];
}

void set_format(struct jpeg_t *jpeg)
{
  int fmt = (jpeg->comp[0].samp_h << 4) | jpeg->comp[0].samp_v;
  if(fmt == 0x11) fmt = 0x1B000000;
  else if(fmt == 0x21) fmt = 0x13000000;
  else if(fmt == 0x12) fmt = 0x23000000;
  else if(fmt == 0x22) fmt = 0x03000000;
  else fmt = 0;
  MPEG->TRIG = fmt;
  //printf("fmt:%x\n", fmt);
}

void set_size(struct jpeg_t *jpeg)
{
  u16 h = (jpeg->height - 1) / (8 * jpeg->comp[0].samp_v);
  u16 w = (jpeg->width - 1) / (8 * jpeg->comp[0].samp_h);
  //uint16_t h = jpeg->height / (8 * jpeg->comp[0].samp_v) - 1;
  //uint16_t w = jpeg->width  / (8 * jpeg->comp[0].samp_h) - 1;
  MPEG->JPEG_SIZE = (u32)h << 16 | w;
}

/* Tables for YCbCr->RGB and BG_YCC->RGB colorspace conversion */
static const s16 crrtab[] = {
  -179, -178, -176, -175, -173, -172, -171, -169, 
  -168, -166, -165, -164, -162, -161, -159, -158, 
  -157, -155, -154, -152, -151, -150, -148, -147, 
  -145, -144, -143, -141, -140, -138, -137, -136, 
  -134, -133, -131, -130, -129, -127, -126, -124, 
  -123, -122, -120, -119, -117, -116, -114, -113, 
  -112, -110, -109, -107, -106, -105, -103, -102, 
  -100, -99, -98, -96, -95, -93, -92, -91, 
  -89, -88, -86, -85, -84, -82, -81, -79, 
  -78, -77, -75, -74, -72, -71, -70, -68, 
  -67, -65, -64, -63, -61, -60, -58, -57, 
  -56, -54, -53, -51, -50, -49, -47, -46, 
  -44, -43, -42, -40, -39, -37, -36, -35, 
  -33, -32, -30, -29, -28, -26, -25, -23, 
  -22, -21, -19, -18, -16, -15, -14, -12, 
  -11, -9, -8, -7, -5, -4, -2, -1, 
  0, 1, 2, 4, 5, 7, 8, 9, 
  11, 12, 14, 15, 16, 18, 19, 21, 
  22, 23, 25, 26, 28, 29, 30, 32, 
  33, 35, 36, 37, 39, 40, 42, 43, 
  44, 46, 47, 49, 50, 51, 53, 54, 
  56, 57, 58, 60, 61, 63, 64, 65, 
  67, 68, 70, 71, 72, 74, 75, 77, 
  78, 79, 81, 82, 84, 85, 86, 88, 
  89, 91, 92, 93, 95, 96, 98, 99, 
  100, 102, 103, 105, 106, 107, 109, 110, 
  112, 113, 114, 116, 117, 119, 120, 122, 
  123, 124, 126, 127, 129, 130, 131, 133, 
  134, 136, 137, 138, 140, 141, 143, 144, 
  145, 147, 148, 150, 151, 152, 154, 155, 
  157, 158, 159, 161, 162, 164, 165, 166, 
  168, 169, 171, 172, 173, 175, 176, 178
};

static const s16 crgtab[] = {
  -91, -90, -89, -89, -88, -87, -87, -86, 
  -85, -84, -84, -83, -82, -82, -81, -80, 
  -79, -79, -78, -77, -77, -76, -75, -74, 
  -74, -73, -72, -72, -71, -70, -69, -69, 
  -68, -67, -67, -66, -65, -64, -64, -63, 
  -62, -62, -61, -60, -59, -59, -58, -57, 
  -57, -56, -55, -54, -54, -53, -52, -52, 
  -51, -50, -49, -49, -48, -47, -47, -46, 
  -45, -44, -44, -43, -42, -42, -41, -40, 
  -39, -39, -38, -37, -37, -36, -35, -34, 
  -34, -33, -32, -32, -31, -30, -29, -29, 
  -28, -27, -27, -26, -25, -24, -24, -23, 
  -22, -22, -21, -20, -19, -19, -18, -17, 
  -17, -16, -15, -14, -14, -13, -12, -12, 
  -11, -10, -9, -9, -8, -7, -7, -6, 
  -5, -4, -4, -3, -2, -2, -1, 0, 
  0, 0, 1, 2, 2, 3, 4, 4, 
  5, 6, 7, 7, 8, 9, 9, 10, 
  11, 12, 12, 13, 14, 14, 15, 16, 
  17, 17, 18, 19, 19, 20, 21, 22, 
  22, 23, 24, 24, 25, 26, 27, 27, 
  28, 29, 29, 30, 31, 32, 32, 33, 
  34, 34, 35, 36, 37, 37, 38, 39, 
  39, 40, 41, 42, 42, 43, 44, 44, 
  45, 46, 47, 47, 48, 49, 49, 50, 
  51, 52, 52, 53, 54, 54, 55, 56, 
  57, 57, 58, 59, 59, 60, 61, 62, 
  62, 63, 64, 64, 65, 66, 67, 67, 
  68, 69, 69, 70, 71, 72, 72, 73, 
  74, 74, 75, 76, 77, 77, 78, 79, 
  79, 80, 81, 82, 82, 83, 84, 84, 
  85, 86, 87, 87, 88, 89, 89, 90
};

static const s16 cbbtab[] = {
  -226, -225, -223, -221, -219, -217, -216, -214, 
  -212, -210, -209, -207, -205, -203, -202, -200, 
  -198, -196, -194, -193, -191, -189, -187, -186, 
  -184, -182, -180, -178, -177, -175, -173, -171, 
  -170, -168, -166, -164, -163, -161, -159, -157, 
  -155, -154, -152, -150, -148, -147, -145, -143, 
  -141, -139, -138, -136, -134, -132, -131, -129, 
  -127, -125, -124, -122, -120, -118, -116, -115, 
  -113, -111, -109, -108, -106, -104, -102, -101, 
  -99, -97, -95, -93, -92, -90, -88, -86, 
  -85, -83, -81, -79, -77, -76, -74, -72, 
  -70, -69, -67, -65, -63, -62, -60, -58, 
  -56, -54, -53, -51, -49, -47, -46, -44, 
  -42, -40, -38, -37, -35, -33, -31, -30, 
  -28, -26, -24, -23, -21, -19, -17, -15, 
  -14, -12, -10, -8, -7, -5, -3, -1, 
  0, 1, 3, 5, 7, 8, 10, 12, 
  14, 15, 17, 19, 21, 23, 24, 26, 
  28, 30, 31, 33, 35, 37, 38, 40, 
  42, 44, 46, 47, 49, 51, 53, 54, 
  56, 58, 60, 62, 63, 65, 67, 69, 
  70, 72, 74, 76, 77, 79, 81, 83, 
  85, 86, 88, 90, 92, 93, 95, 97, 
  99, 101, 102, 104, 106, 108, 109, 111, 
  113, 115, 116, 118, 120, 122, 124, 125, 
  127, 129, 131, 132, 134, 136, 138, 139, 
  141, 143, 145, 147, 148, 150, 152, 154, 
  155, 157, 159, 161, 163, 164, 166, 168, 
  170, 171, 173, 175, 177, 178, 180, 182, 
  184, 186, 187, 189, 191, 193, 194, 196, 
  198, 200, 202, 203, 205, 207, 209, 210, 
  212, 214, 216, 217, 219, 221, 223, 225
};

static const s16 cbgtab[] = {
  -44, -43, -43, -42, -42, -42, -41, -41, 
  -41, -40, -40, -40, -39, -39, -39, -38, 
  -38, -38, -37, -37, -37, -36, -36, -36, 
  -35, -35, -35, -34, -34, -34, -33, -33, 
  -33, -32, -32, -31, -31, -31, -30, -30, 
  -30, -29, -29, -29, -28, -28, -28, -27, 
  -27, -27, -26, -26, -26, -25, -25, -25, 
  -24, -24, -24, -23, -23, -23, -22, -22, 
  -22, -21, -21, -20, -20, -20, -19, -19, 
  -19, -18, -18, -18, -17, -17, -17, -16, 
  -16, -16, -15, -15, -15, -14, -14, -14, 
  -13, -13, -13, -12, -12, -12, -11, -11, 
  -11, -10, -10, -9, -9, -9, -8, -8, 
  -8, -7, -7, -7, -6, -6, -6, -5, 
  -5, -5, -4, -4, -4, -3, -3, -3, 
  -2, -2, -2, -1, -1, -1, 0, 0, 
  0, 0, 0, 1, 1, 1, 2, 2, 
  2, 3, 3, 3, 4, 4, 4, 5, 
  5, 5, 6, 6, 6, 7, 7, 7, 
  8, 8, 8, 9, 9, 9, 10, 10, 
  11, 11, 11, 12, 12, 12, 13, 13, 
  13, 14, 14, 14, 15, 15, 15, 16, 
  16, 16, 17, 17, 17, 18, 18, 18, 
  19, 19, 19, 20, 20, 20, 21, 21, 
  22, 22, 22, 23, 23, 23, 24, 24, 
  24, 25, 25, 25, 26, 26, 26, 27, 
  27, 27, 28, 28, 28, 29, 29, 29, 
  30, 30, 30, 31, 31, 31, 32, 32, 
  33, 33, 33, 34, 34, 34, 35, 35, 
  35, 36, 36, 36, 37, 37, 37, 38, 
  38, 38, 39, 39, 39, 40, 40, 40, 
  41, 41, 41, 42, 42, 42, 43, 43
};

int yuv2rgb565 (u8 Y, u8 Cb, u8 Cr)
{
  int color, rgb;
  color = Y + crrtab[Cr];
  if(color < 0) color = 0; else if(color > 255) color = 255;
  rgb = (color & 0xF8) << 8;
  color = Y - cbgtab[Cb] - crgtab[Cr];
  if(color < 0) color = 0; else if(color > 255) color = 255;
  rgb |= (color & 0xFC) << 3;
  color = Y +cbbtab[Cb];
  if(color < 0) color = 0; else if(color > 255) color = 255;
  return rgb | (color >> 3);
}

void yuv2rgb(u16* out, int width, int height, u8* luma, u8* chroma, int samp)
{
  int x, y, cy;
  for(y = 0; y < height; y++)
  {
    cy = y / samp;
    cy = ((cy % 32) * 32) + ((cy / 32) * (((width + 31) / 32) * 1024));
    for(x = 0; x < width; x += 4)
    {
      u32 Y = *((u32*)(luma + (x / 32) * 1024 + (x % 32) + ((y % 32) * 32) + ((y / 32) * (((width + 31) / 32) * 1024))));
      u32 C = *((u32*)(chroma + (x / 32) * 1024 + ((x % 32) / 2 * 2) + cy));
      *out++ = yuv2rgb565(Y, C, C >> 8);
      *out++ = yuv2rgb565(Y >> 8, C, C >> 8);
      *out++ = yuv2rgb565(Y >> 16, C >> 16, C >> 24);
      *out++ = yuv2rgb565(Y >> 24, C >> 16, C >> 24);
    }
  }
}

void decode_jpeg(struct jpeg_t *jpeg, u16 *frame)
{
  int input_size =(jpeg->data_len + 65535) & ~65535;
  int output_size = ((jpeg->width + 31) & ~31) * ((jpeg->height + 31) & ~31);
  u8 *input_buffer = memalign(1024, input_size);
  u8 *luma_output = memalign(1024, output_size);
  u8 *chroma_output = memalign(1024, output_size);
  u32 ctr_ms = 0;
  printf("decoding\n");
  VE->CTRL = 0x00000000;                        // activate MPEG engine
  memcpy(input_buffer, jpeg->data, jpeg->data_len);
  MPEG->JPEG_RES_INT = jpeg->restart_interval;
  set_format(jpeg);                             // set JPEG format
  MPEG->ROT_LUMA = (u32)luma_output;       // set output buffers (Luma / Croma)
  MPEG->ROT_CHROMA = (u32)chroma_output;
  set_size(jpeg);                               // set size
  MPEG->ROT_SCALE_CTRL = 0;                     // scale 1:1
  MPEG->VLD_END = (u32)input_buffer + input_size - 1;  // input end
  MPEG->VLD_OFFSET = 0;// * 8;                     // set input offset in bits
  MPEG->VLD_LEN = jpeg->data_len * 8;           // set input length in bits
  MPEG->VLD_ADDR = 0x70000000 | ((u32)input_buffer & 0x0ffffff0) | (u32)input_buffer >> 28;
  set_quantization_tables(jpeg);                // set Quantisation Table
  MPEG->RAM_WRITE_PTR = 0;                      // set Huffman Table
  set_huffman_tables(jpeg);
  MPEG->TRIG |= 0x8000000E;                             // Start MPEGs: 0xe, JPEG: 0xd ..
  for(u32 tm = ctr_ms + 1000; ctr_ms < tm; )
  {
    delay_ms(1);
    ctr_ms++;
    if(MPEG->STATUS & 3) break;
  }
  MPEG->STATUS = 0x0000c00f;                    // clean interrupt flag
  VE->CTRL = 7;
  printf("JPGDEC: %umS\t", ctr_ms);
  /* Make rgb565 */
  ctr_ms = 0;
  printf("start: %umS\t", Sagittarius_run_time());
  yuv2rgb(frame, jpeg->width, jpeg->height, luma_output, chroma_output, jpeg->comp[0].samp_v);
  free(chroma_output);
  free(luma_output);
  free(input_buffer);
  printf("YUV2RGB: %umS\r", Sagittarius_run_time());
}

int test_decode_jpeg_demo (void)
{
  int i;
  static int fri = 0;
  struct jpeg_t jpeg;
  u16 *RGB565data = NULL;
  u16 color = 0;

  int error = 0;
  unsigned char *_fptr = NULL;
  size_t file_size = 0;
  error = MTF_load_file(&_fptr, &file_size, "./MTF/15.jpg");
  if(error)
    return 1;
printf("show ready\n");

  CCU->PLL_VE_CTRL = (1u << 31) | (1 << 24) | (12 << 8); // 312MHz
  while(!(CCU->PLL_VE_CTRL & (1 << 28)));
  CCU->VE_CLK |= (1u << 31);
  CCU->BUS_CLK_GATING1 |= 1;
  CCU->BUS_SOFT_RST1 &= ~1;
  CCU->BUS_SOFT_RST1 |= 1;
  CCU->DRAM_GATING |= 1;
  SYS->CTRL[0] &= 0x80000000;
  SYS->CTRL[0] |= 0x7fffffff;
  SYS->CTRL[1] &= 0xefffffff;
  printf("VE Version 0x%X 0x%X\n", VE->VERSION >> 16, VE->CTRL);
  VE->CTRL = 7;
  delay(50);

  if (_fptr[0] != 0xFF)
      return 0;
  if (_fptr[1] != 0xD8)
      return 0;
  for (i = 2; i < file_size; i++)
  {
      if (_fptr[i] == 0xFF && _fptr[i + 1] == 0xD9)
          break;
  }
  if (_fptr[i] != 0xFF && _fptr[i + 1] != 0xD9)
      return 0;
  i += 2;
  if (!parse_jpeg(&jpeg, _fptr, i))
  {
      printf("Can't parse JPEG");
      return 0;
  }
  fri ^= 1;
  RGB565data = malloc(jpeg.width*jpeg.height*sizeof(u16));
  decode_jpeg(&jpeg, RGB565data); //解码
    //显示
    printf("show jpeg: w %d, h %d\n", jpeg.width, jpeg.height);
    	for (i = 0; i < jpeg.width*jpeg.height; i++)
	{
        color = RGB565data[i];
		((ColorClass *)lcddev.pixelDatas)[i] = RGB565to888(&color);
	}
  free(RGB565data);
  return i;
}
