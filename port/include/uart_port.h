/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_PORT_H
#define __UART_PORT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "types_plus.h"
#include "uart-16550.h"

/** @addtogroup UART
  * @{
  */
#define MTF_UART_0 UART0
#define MTF_UART_1 UART1
#define MTF_UART_2 UART2

/* Exported types ------------------------------------------------------------*/
/** @defgroup UART_Exported_Types UART Exported Types
  * @{
  */

/**
  * @brief UART Init Structure definition
  */
typedef struct
{
  uint32_t BaudRate;                  /*!< This member configures the UART communication baud rate.
                                           The baud rate register is computed using the following formula:
                                           - If oversampling is 16 or in LIN mode,
                                              Baud Rate Register = ((PCLKx) / ((huart->Init.BaudRate)))
                                           - If oversampling is 8,
                                              Baud Rate Register[15:4] = ((2 * PCLKx) / ((huart->Init.BaudRate)))[15:4]
                                              Baud Rate Register[3] =  0
                                              Baud Rate Register[2:0] =  (((2 * PCLKx) / ((huart->Init.BaudRate)))[3:0]) >> 1      */

  uint32_t WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UARTEx_Word_Length */

  uint32_t StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

  uint32_t Parity;                    /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref UART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */

  uint32_t Mode;                      /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Mode */

  uint32_t HwFlowCtl;                 /*!< Specifies whether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref UART_Hardware_Flow_Control */

  uint32_t OverSampling;              /*!< Specifies whether the Over sampling 8 is enabled or disabled, to achieve higher speed (up to fPCLK/8).
                                           This parameter can be a value of @ref UART_Over_Sampling */

  uint32_t OneBitSampling;            /*!< Specifies whether a single sample or three samples' majority vote is selected.
                                           Selecting the single sample method increases the receiver tolerance to clock
                                           deviations. This parameter can be a value of @ref UART_OneBit_Sampling */
}MTF_UART_InitDef;

/**
  * @brief  UART Advanced Features initialization structure definition
  */

/**
  * @brief HAL UART State structures definition
  * @note  HAL UART State value is a combination of 2 different substates: gState and RxState.
  *        - gState contains UART state information related to global Handle management 
  *          and also information related to Tx operations.
  *          gState value coding follow below described bitmap :
  *          b7-b6  Error information 
  *             00 : No Error
  *             01 : (Not Used)
  *             10 : Timeout
  *             11 : Error
  *          b5     IP initilisation status
  *             0  : Reset (IP not initialized)
  *             1  : Init done (IP not initialized. HAL UART Init function already called)
  *          b4-b3  (not used)
  *             xx : Should be set to 00
  *          b2     Intrinsic process state
  *             0  : Ready
  *             1  : Busy (IP busy with some configuration or internal operations)
  *          b1     (not used)
  *             x  : Should be set to 0
  *          b0     Tx state
  *             0  : Ready (no Tx operation ongoing)
  *             1  : Busy (Tx operation ongoing)
  *        - RxState contains information related to Rx operations.
  *          RxState value coding follow below described bitmap :
  *          b7-b6  (not used)
  *             xx : Should be set to 00
  *          b5     IP initilisation status
  *             0  : Reset (IP not initialized)
  *             1  : Init done (IP not initialized)
  *          b4-b2  (not used)
  *            xxx : Should be set to 000
  *          b1     Rx state
  *             0  : Ready (no Rx operation ongoing)
  *             1  : Busy (Rx operation ongoing)
  *          b0     (not used)
  *             x  : Should be set to 0.
  */
typedef enum
{
  MTF_UART_STATE_RESET             = 0x00U,   /*!< Peripheral is not initialized
                                                   Value is allowed for gState and RxState */
  MTF_UART_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use
                                                   Value is allowed for gState and RxState */
  MTF_UART_STATE_BUSY              = 0x24U,   /*!< an internal process is ongoing 
                                                   Value is allowed for gState only */
  MTF_UART_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing
                                                   Value is allowed for gState only */
  MTF_UART_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing
                                                   Value is allowed for RxState only */
  MTF_UART_STATE_BUSY_TX_RX        = 0x23U,   /*!< Data Transmission and Reception process is ongoing
                                                   Not to be used for neither gState nor RxState.
                                                   Value is result of combination (Or) between gState and RxState values */
  MTF_UART_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state
                                                   Value is allowed for gState only */
  MTF_UART_STATE_ERROR             = 0xE0U    /*!< Error
                                                   Value is allowed for gState only */
}MTF_UART_StateDef;

/**
  * @brief UART clock sources definition
  */

/**
  * @brief  UART handle Structure definition
  */
typedef struct
{
  dev_addr addr; //设备地址
  MTF_UART_InitDef         Init;             /*!< UART communication parameters      */
  struct uart_t * uart;
}MTF_HandleDef;

/**
  * @}
  */

/**
  * @}
  */
/** @defgroup UART_Stop_Bits   UART Number of Stop Bits
  * @{
  */
#define MTF_UART_STOPBITS_1                     1
#define MTF_UART_STOPBITS_2                     2
/**
  * @}
  */
#define MTF_UART_WORDLENGTH_7B                  7
#define MTF_UART_WORDLENGTH_8B                  8
#define MTF_UART_WORDLENGTH_9B                  9

/** @defgroup UART_Parity  UART Parity
  * @{
  */
#define MTF_UART_PARITY_NONE                    0
#define MTF_UART_PARITY_EVEN                    1
#define MTF_UART_PARITY_ODD                     2
/**
  * @}
  */

/** @defgroup UART_Hardware_Flow_Control UART Hardware Flow Control
  * @{
  */
#define MTF_UART_HWCONTROL_NONE                  0
#define MTF_UART_HWCONTROL_RTS                   1
#define MTF_UART_HWCONTROL_CTS                   2
#define MTF_UART_HWCONTROL_RTS_CTS               3
/**
  * @}
  */

/** @defgroup UART_Mode UART Transfer Mode
  * @{
  */
#define MTF_UART_MODE_RX                        0
#define MTF_UART_MODE_TX                        1
#define MTF_UART_MODE_TX_RX                     2
/**
  * @}
  */

/**
  * @}
  */

/**       Elements values convention: 0000ZZZZ00000000b
  *           - ZZZZ  : Flag position in the ISR register(4bits)
  */

/**
  * @}
  */

/**
  * @}
  */
/* Include UART HAL Extension module */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup UART_Exported_Functions UART Exported Functions
  * @{
  */

/** @addtogroup UART_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */

/* Initialization and de-initialization functions  ****************************/
HAL_StatusTypeDef MTF_UART_Init(MTF_HandleDef *huart);
HAL_StatusTypeDef MTF_UART_Reset(MTF_HandleDef *huart);

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group2 IO operation functions
  * @{
  */

/* IO operation functions *****************************************************/
size_t MTF_UART_Transmit(MTF_HandleDef *huart, uint8_t *pData, size_t Size);
size_t MTF_UART_Receive(MTF_HandleDef *huart, uint8_t *pData, size_t Size);
HAL_StatusTypeDef MTF_UART_Transmit_IT(MTF_HandleDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MTF_UART_Receive_IT(MTF_HandleDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MTF_UART_Transmit_DMA(MTF_HandleDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MTF_UART_Receive_DMA(MTF_HandleDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef MTF_UART_DMAPause(MTF_HandleDef *huart);
HAL_StatusTypeDef MTF_UART_DMAResume(MTF_HandleDef *huart);
HAL_StatusTypeDef MTF_UART_DMAStop(MTF_HandleDef *huart);
uint8_t MTF_UART_Transmit_Empty(MTF_HandleDef *huart);
uint8_t MTF_UART_Receive_Empty(MTF_HandleDef *huart);
uint8_t MTF_UART_Transmit_FIFO_Count(MTF_HandleDef *huart);

static __INLINE uint8_t MTF_UART_Receive_FIFO_Count(MTF_HandleDef *huart)
{
  return uart_16550_rec_fifo_cnt(huart->uart);
}

void MTF_UART_IRQHandler(MTF_HandleDef *huart);
void MTF_UART_TxHalfCpltCallback(MTF_HandleDef *huart);
void MTF_UART_TxCpltCallback(MTF_HandleDef *huart);
void MTF_UART_RxHalfCpltCallback(MTF_HandleDef *huart);
void MTF_UART_RxCpltCallback(MTF_HandleDef *huart);
void MTF_UART_ErrorCallback(MTF_HandleDef *huart);

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group3 Peripheral Control functions
  * @{
  */

/* Peripheral Control functions  ************************************************/


/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group4 Peripheral State and Error functions
  * @{
  */

/* Peripheral State and Errors functions  **************************************************/
MTF_UART_StateDef MTF_UART_GetState(MTF_HandleDef *huart);
uint32_t MTF_UART_GetError(MTF_HandleDef *huart);

/**
  * @}
  */

/**
  * @}
  */

/* Private functions -----------------------------------------------------------*/
/** @addtogroup UART_Private_Functions UART Private Functions
  * @{
  */

/****************************
应用定义接口
*****************************/
#define UART_D_HMI MTF_UART_0 

#ifdef __cplusplus
}
#endif

#endif /* __STM32F7xx_HAL_UART_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
