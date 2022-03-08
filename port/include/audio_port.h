#ifndef _AUDIO_PORT_H
#define _AUDIO_PORT_H

#include "types_base.h"

/**************SDL2 define*************/
#define AUDIO_U16SYS 16
#define AUDIO_S16SYS 16
#define AUDIO_S24SYS 24
#define AUDIO_S32SYS 32
#define AUDIO_F32SYS 32

typedef uint16_t SDL_AudioFormat;

typedef void (* SDL_AudioCallback) (void *userdata, uint8_t * stream,
                                            int len);

typedef struct
{
    int freq;                   /**< DSP frequency -- samples per second */
    SDL_AudioFormat format;     /**< Audio data format */
    uint8_t channels;             /**< Number of channels: 1 mono, 2 stereo */
    uint8_t silence;              /**< Audio buffer silence value (calculated) */
    uint16_t samples;             /**< Audio buffer size in sample FRAMES (total samples divided by channel count) */
    uint16_t padding;             /**< Necessary for some compile environments */
    uint32_t size;                /**< Audio buffer size in bytes (calculated) */
	SDL_AudioCallback callback; /**< Callback that feeds the audio device (NULL to use SDL_QueueAudio()). */
    void *userdata;             /**< Userdata passed to callback (ignored for NULL callbacks). */
} SDL_AudioSpec;
/****************************************/

typedef SDL_AudioSpec audio_pcm_dev;

void MTF_audio_time(void);
uint32_t MTF_audio_output_time_get(void);
char MTF_audio_pcm_output_init(audio_pcm_dev *dev, audio_pcm_dev *dest);
char MTF_audio_pcm_output_exit(audio_pcm_dev *dev);
uint8_t MTF_audio_pcm_output_busy(audio_pcm_dev *dev);
uint8_t MTF_audio_pcm_output(audio_pcm_dev *dev, uint8_t *stream, int len);
void MTF_audio_vol(unsigned char i);

#endif
