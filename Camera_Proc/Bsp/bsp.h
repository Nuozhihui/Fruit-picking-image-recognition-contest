/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                            (c) Copyright 2007-2008; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

#ifndef __BSP_H__
#define __BSP_H__

#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 

/* Private typedef -----------------------------------------------------------*/
// USART 
typedef enum 
{   FAILED = 0,
    PASSED = !FAILED
}   TestStatus;
// LED 
typedef enum 
{
  LED_ALL = 0,
  LED1 = 1,
  LED2 = 2
} Led_TypeDef;
/* Private define ------------------------------------------------------------*/
// UART1~UART5 BufferSize
#define TxBufferSize1   16 //(countof(TxBuffer1) - 1)
#define RxBufferSize1   16 //(countof(TxBuffer1) - 1)
//#define TxBufferSize2   16 
//#define RxBufferSize2   16 
//#define TxBufferSize3   16 
//#define RxBufferSize3   16 
#define TxBufferSize4   16 
#define RxBufferSize4   16 
#define TxBufferSize5   16 
#define RxBufferSize5   16 

// UART1~UART5 IO∂®“Â
#define BSP_IO_TXD1     GPIO_Pin_9	 // PA9(IO101)
#define BSP_IO_RXD1     GPIO_Pin_10	 // PA10(IO102)
#define BSP_IO_DIR1     GPIO_Pin_8   // PA8(IO100)

#define BSP_IO_TXD4     GPIO_Pin_10	 // PC10(IO111)
#define BSP_IO_RXD4     GPIO_Pin_11	 // PC11(IO112)	


#define BSP_IO_TXD5     GPIO_Pin_12	 // PC12(IO113)
#define BSP_IO_RXD5     GPIO_Pin_2	 // PD2(IO116)


/* Function  -----------------------------------------------------------*/
void BSP_Configuration(void);
void BSP_GPIO_Configuration(void);
//void Alarm_Ctrl(void);
void BSP_EXTI_Configuration(void);
void BSP_USART_Configuration(USART_TypeDef* USARTx);
void BSP_USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len);
void BSP_RCC_Configuration(void);
//void key_scan(void);
//void BSP_LED_Off (Led_TypeDef LED) ;
//void BSP_LED_On (Led_TypeDef LED);
//void BSP_LED_Toggle (Led_TypeDef LED);
//void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
extern void UART4_IRQHandler(void); 

#endif 
                               
