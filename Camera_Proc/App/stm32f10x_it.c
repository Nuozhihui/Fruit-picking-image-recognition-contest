#include "stm32f10x_it.h"
#include  "bsp.h"




/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */

void SysTick_Handler(void)
{
  TimingDelay_Decrement();
}



/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval : None
  */
/*
void UART4_IRQHandler(void)
{
   	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
 	 {	
  		 RxData4 = USART_ReceiveData(UART4);   //将读寄存器的数据缓存到接收缓冲区里
		if(RxData4 == 0x55)	FLAG_1=1;

 	 }

	if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
 	 { 
    	 USART_ITConfig(UART4, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
 	 }	
}
*/
/*
void UART5_IRQHandler(void)
{ 
 	unsigned int i;
   	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
 	 {	
  		 RxBuffer5[RxCounter5++] = USART_ReceiveData(UART5);   //将读寄存器的数据缓存到接收缓冲区里
	
    	//if(RxBuffer4[RxCounter4-1]==0xaa)     //判断结束标志是否是 0xaa
		//if(RxCounter4 == NbrOfDataToRead4)
		if(RxCounter5 == 0x04)
    	{
	  		for(i=0; i< RxCounter5; i++) TxBuffer5[i]	= RxBuffer5[i]; 	     //将接收缓冲器的数据转到发送缓冲区，准备转发
	  		rec_f5=1;															 //接收成功标志
	  		//TxBuffer4[RxCounter4]=0;		                                     //发送缓冲区结束符    
	  		RxCounter5=0;
	  	}

 	 }



    	//USART_SendData(UART4,USART_ReceiveData(UART4));    //判断结束标志是否是0x0d 0x0a
   		// for(i==0;i<500;i++);
  //	}
	if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
 	 { 
    	 USART_ITConfig(UART5, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
 	 }	
}
*/


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

