/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210E-EVAL Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "bsp.h"
extern __IO uint32_t TimingDelay ;

extern u8 FLAG_1;

extern u8 Send_Step;//Send_Flag
extern u16 Grid_flag; 

/*
*********************************************************************************************************
* Function    ：void BSP_Configuration(void)
* Description : 硬件初始化 
* Para        : none
* Return(s)   : none
* Example     : BSP_Configuration();
*********************************************************************************************************
*/
void BSP_Configuration(void)
{
	BSP_RCC_Configuration();
    BSP_GPIO_Configuration(); 
	BSP_EXTI_Configuration();
	//BSP_USART_Configuration();
}



void BSP_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*********USART TX**************/
	GPIO_InitStructure.GPIO_Pin = BSP_IO_TXD4;	    //UART4/UART5  TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;            //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);		      
    /*********USART RX**************/
    GPIO_InitStructure.GPIO_Pin = BSP_IO_RXD4;                 // UART4	RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       // 复用开漏输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    		        
}
/*
*********************************************************************************************************
* Function    ：void BSP_RCC_Configuration(void)
* Description : 时钟配置函数,系统时钟配置为72MHZ，外设时钟配置 
* Para        : none
* Return(s)   : none
* Example     : BSP_RCC_Configuration();
*********************************************************************************************************
*/
void BSP_RCC_Configuration(void)
{
	SystemInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);// 引脚映射、IO中断进行读写前打开AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // APB2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
}
/*
*********************************************************************************************************
* Function    ：void BSP_EXTI_Configuration(void)
* Description : IO中断 
* Para        : none
* Return(s)   : none
* Example     : none
*********************************************************************************************************
*/
void BSP_EXTI_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //EXTI_InitTypeDef EXTI_InitStructure;
    /* Configure one bit for preemption priority */
    /* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是1， 3 
    0组：  抢占优先级占0位， 响应优先级占4位
    1组：  抢占优先级占1位， 响应优先级占3位
    2组：  抢占优先级占2位， 响应优先级占2位
    3组：  抢占优先级占3位， 响应优先级占1位
    4组:   抢占优先级占4位， 响应优先级占0位  */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;			     //设置串口UART4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;               // 子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
*********************************************************************************************************
* Function    ：void BSP_USART_Configuration(USART_TypeDef* USARTx)
* Description : 串口通信参数设置 
* Para        : none
* Return(s)   : none
* Example     : BSP_USART_Configuration(USART1);
*********************************************************************************************************
*/

void BSP_USART_Configuration(USART_TypeDef* USARTx)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = 115200;					//速率115200bps
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
    USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

    /* Configure USART1 */
    USART_Init(USARTx, &USART_InitStructure);							//配置串口参数函数
 
  
    /* Enable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);                      //使能接收中断
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);						//使能发送缓冲空中断
    //USART_ITConfig(USART1, USART_IT_TC, ENABLE);						//使能发送完成中断

    /* Enable the USART1 */
    USART_Cmd(USARTx, ENABLE);	
}
/*
*********************************************************************************************************
* Function    ：void BSP_USART_OUT(void)
* Description : 单片机串口发送数据
* Para        : none
* Return(s)   : none
* Example     : BSP_USART_OUT(USART1, uint8_t *Data,uint16_t Len)
*********************************************************************************************************
*/
void BSP_USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,uint16_t Len)
{ 
	uint16_t i;
	for(i=0; i<Len; i++)
	{
		
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(USARTx, Data[i]);					    
	     
	}
}

void UART4_IRQHandler(void)
{
		static u8 RxCnt;
		uint8_t RxData;
   	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
 	 {	
  		 RxData = USART_ReceiveData(UART4);   //将读寄存器的数据缓存到接收缓冲区里
		 
			 if(RxData==0x81) RxCnt=1;
			 if(RxCnt==2)
			 {
					RxCnt=0;
					if(RxData==0x50) 			{	Send_Step=2; Grid_flag=0;}//发送有目标成功,电机停止;
					else if(RxData==0x52)	{	Send_Step=1;}//发送坐标成功,暂停发送;	Send_Flag=0;
					else if(RxData==0x55)	{	Send_Step=1;}//启动发送数据;Send_Flag=1;	
			 }
			 if(RxCnt==1)	RxCnt++;
			 
			/* if(RxData==0x81) RxCnt=1;
			 if(RxCnt==2)
			 {
					RxCnt=0;
					if(RxData==0x50) 			{	Send_Step=2; Grid_flag=0;}//发送有目标成功,电机停止;
					else if(RxData==0x52)	{	Send_Step=1;}//发送坐标成功,暂停发送;	Send_Flag=0;
					else if(RxData==0x55)	{	Send_Step=1;}//启动发送数据;Send_Flag=1;	
			 }
			 if(RxCnt==1)	RxCnt++;
			 */

 	 }

	if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
 	 { 
    	 USART_ITConfig(UART4, USART_IT_TXE, DISABLE);					     //禁止发缓冲器空中断， 
 	 }	
}





/****************************************************************************
* 名    称：void Delay(__IO uint32_t nTime)
* 功    能：定时延时程序 1ms为单位
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
/*void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}*/
/****************************************************************************
* 名    称：void TimingDelay_Decrement(void)
* 功    能：获取节拍程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


