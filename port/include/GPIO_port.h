/*
Copyright (c) 2019-2023 Aysi 773917760@qq.com. All right reserved
Official site: www.mtf123.club

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

It under the terms of the Apache as published;
either version 2 of the License,
or (at your option) any later version.
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _IO_PIN_DEV_H
#define _IO_PIN_DEV_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "types_plus.h"
#include <gpio-f1c100s.h>

/** @addtogroup GPIO
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Types GPIO Exported Types
  * @{
  */
typedef gpio_port_t MTF_GPIO_PORT;
#define MTF_GPIO_A ((MTF_GPIO_PORT *)&GPIO_PA)
#define MTF_GPIO_B ((MTF_GPIO_PORT *)&GPIO_PB)
#define MTF_GPIO_C ((MTF_GPIO_PORT *)&GPIO_PC)
#define MTF_GPIO_D ((MTF_GPIO_PORT *)&GPIO_PD)
#define MTF_GPIO_E ((MTF_GPIO_PORT *)&GPIO_PE)
#define MTF_GPIO_F ((MTF_GPIO_PORT *)&GPIO_PF)

/** 
  * @brief GPIO Init structure definition  
  */ 
typedef struct
{
  uint32_t Pin;       /*!< Specifies the GPIO pins to be configured.
                           This parameter can be any value of @ref GPIO_pins_define */

  uint32_t Mode;      /*!< Specifies the operating mode for the selected pins.
                           This parameter can be a value of @ref GPIO_mode_define */

  uint32_t Pull;      /*!< Specifies the Pull-up or Pull-Down activation for the selected pins.
                           This parameter can be a value of @ref GPIO_pull_define */

  uint32_t Speed;     /*!< Specifies the speed for the selected pins.
                           This parameter can be a value of @ref GPIO_speed_define */

  uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins. 
                            This parameter can be a value of @ref GPIO_Alternate_function_selection */
}MTF_GPIO_Type;

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  MTF_PIN_RESET = 0,
  MTF_PIN_SET
}MTF_GPIO_State;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants GPIO Exported Constants
  * @{
  */ 

/** @defgroup GPIO_pins_define GPIO pins define
  * @{
  */
#define MTF_PIN_0                 0 //((uint16_t)0x0001U)  /* Pin 0 selected    */
#define MTF_PIN_1                 1 //((uint16_t)0x0002U)  /* Pin 1 selected    */
#define MTF_PIN_2                 2 //((uint16_t)0x0004U)  /* Pin 2 selected    */
#define MTF_PIN_3                 3 //((uint16_t)0x0008U)  /* Pin 3 selected    */
#define MTF_PIN_4                 4 //((uint16_t)0x0010U)  /* Pin 4 selected    */
#define MTF_PIN_5                 5 //((uint16_t)0x0020U)  /* Pin 5 selected    */
#define MTF_PIN_6                 6 //((uint16_t)0x0040U)  /* Pin 6 selected    */
#define MTF_PIN_7                 7 //((uint16_t)0x0080U)  /* Pin 7 selected    */
#define MTF_PIN_8                 8 //((uint16_t)0x0100U)  /* Pin 8 selected    */
#define MTF_PIN_9                 9 //((uint16_t)0x0200U)  /* Pin 9 selected    */
#define MTF_PIN_10                10 //((uint16_t)0x0400U)  /* Pin 10 selected   */
#define MTF_PIN_11                11 //((uint16_t)0x0800U)  /* Pin 11 selected   */
#define MTF_PIN_12                12 //((uint16_t)0x1000U)  /* Pin 12 selected   */
#define MTF_PIN_13                13 //((uint16_t)0x2000U)  /* Pin 13 selected   */
#define MTF_PIN_14                14 //((uint16_t)0x4000U)  /* Pin 14 selected   */
#define MTF_PIN_15                15 //((uint16_t)0x8000U)  /* Pin 15 selected   */
#define MTF_PIN_All               0XFFFF //((uint16_t)0xFFFFU)  /* All pins selected */

#define MTF_PIN_MASK              ((uint32_t)0x0000FFFFU) /* PIN mask for assert test */
/**
  * @}
  */

/** @defgroup GPIO_mode_define GPIO mode define
  * @brief GPIO Configuration Mode 
  *        Elements values convention: 0xX0yz00YZ
  *           - X  : GPIO mode or EXTI Mode
  *           - y  : External IT or Event trigger detection 
  *           - z  : IO configuration on External IT or Event
  *           - Y  : Output type (Push Pull or Open Drain)
  *           - Z  : IO Direction mode (Input, Output, Alternate or Analog)
  * @{
  */ 
#define  MTF_PIN_MODE_INPUT                        ((uint32_t)0x00000000U)   /*!< Input Floating Mode                   */
#define  MTF_PIN_MODE_OUTPUT_PP                    ((uint32_t)0x00000001U)   /*!< Output Push Pull Mode                 */
#define  MTF_PIN_MODE_OUTPUT_OD                    ((uint32_t)0x00000011U)   /*!< Output Open Drain Mode                */
#define  MTF_PIN_MODE_AF_PP                        ((uint32_t)0x00000002U)   /*!< Alternate Function Push Pull Mode     */
#define  MTF_PIN_MODE_AF_OD                        ((uint32_t)0x00000012U)   /*!< Alternate Function Open Drain Mode    */

#define  MTF_PIN_MODE_ANALOG                       ((uint32_t)0x00000003U)   /*!< Analog Mode  */
    
#define  MTF_PIN_MODE_IT_RISING                    ((uint32_t)0x10110000U)   /*!< External Interrupt Mode with Rising edge trigger detection          */
#define  MTF_PIN_MODE_IT_FALLING                   ((uint32_t)0x10210000U)   /*!< External Interrupt Mode with Falling edge trigger detection         */
#define  MTF_PIN_MODE_IT_RISING_FALLING            ((uint32_t)0x10310000U)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */
 
#define  MTF_PIN_MODE_EVT_RISING                   ((uint32_t)0x10120000U)   /*!< External Event Mode with Rising edge trigger detection               */
#define  MTF_PIN_MODE_EVT_FALLING                  ((uint32_t)0x10220000U)   /*!< External Event Mode with Falling edge trigger detection              */
#define  MTF_PIN_MODE_EVT_RISING_FALLING           ((uint32_t)0x10320000U)   /*!< External Event Mode with Rising/Falling edge trigger detection       */
/**
  * @}
  */

/** @defgroup GPIO_speed_define  GPIO speed define
  * @brief GPIO Output Maximum frequency
  * @{
  */  
#define  MTF_PIN_SPEED_FREQ_LOW         ((uint32_t)0x00000000U)  /*!< Low speed     */
#define  MTF_PIN_SPEED_FREQ_MEDIUM      ((uint32_t)0x00000001U)  /*!< Medium speed  */
#define  MTF_PIN_SPEED_FREQ_HIGH        ((uint32_t)0x00000002U)  /*!< Fast speed    */
#define  MTF_PIN_SPEED_FREQ_VERY_HIGH   ((uint32_t)0x00000003U)  /*!< High speed    */
/**
  * @}
  */

 /** @defgroup GPIO_pull_define GPIO pull define
   * @brief GPIO Pull-Up or Pull-Down Activation
   * @{
   */  
#define  MTF_PIN_NOPULL        ((uint32_t)0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define  MTF_PIN_PULLUP        ((uint32_t)0x00000001U)   /*!< Pull-up activation                  */
#define  MTF_PIN_PULLDOWN      ((uint32_t)0x00000002U)   /*!< Pull-down activation                */
/**
  * @}
  */
  
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup GPIO_Exported_Functions
  * @{
  */

/** @addtogroup GPIO_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions *****************************/
void  MTF_GPIO_Init(MTF_GPIO_PORT  *GPIOx, MTF_GPIO_Type *GPIO_Init);
void  MTF_GPIO_DeInit(MTF_GPIO_PORT  *GPIOx, uint32_t GPIO_Pin);
/**
  * @}
  */

/** @addtogroup GPIO_Exported_Functions_Group2
  * @{
  */
/* IO operation functions *****************************************************/
static __INLINE MTF_GPIO_State MTF_GPIO_ReadPin(MTF_GPIO_PORT* GPIOx, uint16_t GPIO_Pin)
{
    return gpio_f1c100s_get_value(GPIOx, GPIO_Pin);
}

static __INLINE void MTF_GPIO_WritePin(MTF_GPIO_PORT* GPIOx, uint16_t GPIO_Pin, MTF_GPIO_State PinState)
{
    gpio_f1c100s_set_value(GPIOx, GPIO_Pin, PinState);
}

static __INLINE void MTF_GPIO_TogglePin(MTF_GPIO_PORT* GPIOx, uint16_t GPIO_Pin)
{
    gpio_f1c100s_set_value(GPIOx, GPIO_Pin, !gpio_f1c100s_get_value(GPIOx, GPIO_Pin));
}

static __INLINE void MTF_GPIO_SetDir(MTF_GPIO_PORT* GPIOx, uint16_t GPIO_Pin, gpio_direction_t i)
{
    gpio_f1c100s_set_dir(GPIOx, GPIO_Pin, i);
}

HAL_StatusTypeDef MTF_GPIO_LockPin(MTF_GPIO_PORT* GPIOx, uint16_t GPIO_Pin);
void MTF_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
void MTF_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* _IO_PIN_DEV_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
