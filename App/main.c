/*
 * @file          mian.c
 * @brief         主函数程序
 * @ucos-ii       V2.86 
 * @function      DCP-28 
 * @compiler      RealView MDK-ARM Compiler V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2018/04/27
 * @corporation   浙江求是科教设备有限公司
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "bsp.h"
#include  "adc.h"
#include  "PWM.h"
#include  "motor.h"
#include  "LCD12864.h"
#include  "keypad.h"
#include  "Adjust.h"
#include  "operate.h"
/* Private variables ---------------------------------------------------------*/


// 主程序部分;
int main(void)
{
    BSP_Configuration();			//硬件初始化 1. 时钟 2.鸣器,摇杆电位器(包括按键）,矩阵键盘 HL,电机使能,串口 初始化 3.串口
	
		// 显示部分;
		GPIO_12864_initout();			//屏幕硬件初始化  , 	SW独立按键初始化	
		TIM2_12864_Init();				//TM2定时器初始化
	
			// PWM部分;
		TIM1_PWM_Init();					//	GPIE12 (OUT A)	GPIOE9 (OUT B)	 GPIE8 (OUT C)	GPIOE11 (OUT D)	
	
		TIM4_PWM_Init();					//	Z轴电机(GPIOB8),舵机(GPIOB9)
	
		TIM3_Cap_Init();					//	TIM3通道3/4输入捕获
	
			// ADC 部分;
		ADC_Configuration();			//ADC
	
			// 串口通信部分;
		BSP_USART_Configuration(UART4);		//串口
		 
			if (SysTick_Config(72000))		//时钟节拍中断时1ms一次  用于定时 
			{
					while (1);
			}
			
		Flash_Init();										// 读取X,Y轴摄像头偏移量（Flash-EEPROM初始化）;
			
		LCD_Init();											//LCD初始化设定
			
		Area_Init();										// 区域设定初始值;			（1号键_区域设置）
			
		Alarm_Ctrl();										//蜂鸣器初始化
			
			

			
		//清屏	
		LCD_Clear_Screen();
	
			
    while (1)// 开机界面;
    {
			
			Key_Event();		//判断矩阵是否按下，获取key_val键值
			
			
			if(key_val==16)					//************X键进入主界面*********		具体搜索  *****按键代码参考表***		左边是定义按键内容  左边按键内ID
			{
				key_val=0;
				LCD_flag = 1;// 跳转主界面;
				Set_flag=0;// 系统启动;
				TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//禁止使能时钟TIM2定时中断源中断
				TIM_Cmd(TIM2, DISABLE);											//禁止使能定时器2
				break;
			}
			
			LCD_Display_Fresh();// 液晶显示	LCD_flag为界面号: 0=>LCD_Start; 1=>LCD_Main; 2=>LCD_Set; 3=>LCD_State; 4=>坐标点图;;
		}
		while (1)// 主程序;
    {
			Key_Event();				//矩阵按键数值读取					****修改按键*****
			key_scan();					//独立按键数值读取	
			
			
			switch(Set_flag)
			{	
				case 0:		Set0_Home();		// 主界面 ;
					break;
				case 1:		Set1_Area();		// 1.区域设定 ;
					break;
				case 2:		Set2_Hand();		// 2.手动采摘 ;
					break;
				case 3:		Set3_Auto();		// 3.自动采摘 ;
					break;
				case 4:		Set4_Adjust();	// 4.校准 ;
					break;
				default:break;
			}
			
			LCD_Display_Fresh();// 液晶显示;
			
		}
}

