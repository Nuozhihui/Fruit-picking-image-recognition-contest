/*
 * @file          mian.c
 * @brief         ����������
 * @ucos-ii       V2.86 
 * @function      DCP-28 
 * @compiler      RealView MDK-ARM Compiler V5.14
 * @peripheral    V3.5.0
 * @author  	  	
 * @date   	      2018/04/27
 * @corporation   �㽭���ǿƽ��豸���޹�˾
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


// �����򲿷�;
int main(void)
{
    BSP_Configuration();			//Ӳ����ʼ�� 1. ʱ�� 2.����,ҡ�˵�λ��(����������,������� HL,���ʹ��,���� ��ʼ�� 3.����
	
		// ��ʾ����;
		GPIO_12864_initout();			//��ĻӲ����ʼ��  , 	SW����������ʼ��	
		TIM2_12864_Init();				//TM2��ʱ����ʼ��
	
			// PWM����;
		TIM1_PWM_Init();					//	GPIE12 (OUT A)	GPIOE9 (OUT B)	 GPIE8 (OUT C)	GPIOE11 (OUT D)	
	
		TIM4_PWM_Init();					//	Z����(GPIOB8),���(GPIOB9)
	
		TIM3_Cap_Init();					//	TIM3ͨ��3/4���벶��
	
			// ADC ����;
		ADC_Configuration();			//ADC
	
			// ����ͨ�Ų���;
		BSP_USART_Configuration(UART4);		//����
		 
			if (SysTick_Config(72000))		//ʱ�ӽ����ж�ʱ1msһ��  ���ڶ�ʱ 
			{
					while (1);
			}
			
		Flash_Init();										// ��ȡX,Y������ͷƫ������Flash-EEPROM��ʼ����;
			
		LCD_Init();											//LCD��ʼ���趨
			
		Area_Init();										// �����趨��ʼֵ;			��1�ż�_�������ã�
			
		Alarm_Ctrl();										//��������ʼ��
			
			

			
		//����	
		LCD_Clear_Screen();
	
			
    while (1)// ��������;
    {
			
			Key_Event();		//�жϾ����Ƿ��£���ȡkey_val��ֵ
			
			
			if(key_val==16)					//************X������������*********		��������  *****��������ο���***		����Ƕ��尴������  ��߰�����ID
			{
				key_val=0;
				LCD_flag = 1;// ��ת������;
				Set_flag=0;// ϵͳ����;
				TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//��ֹʹ��ʱ��TIM2��ʱ�ж�Դ�ж�
				TIM_Cmd(TIM2, DISABLE);											//��ֹʹ�ܶ�ʱ��2
				break;
			}
			
			LCD_Display_Fresh();// Һ����ʾ	LCD_flagΪ�����: 0=>LCD_Start; 1=>LCD_Main; 2=>LCD_Set; 3=>LCD_State; 4=>�����ͼ;;
		}
		while (1)// ������;
    {
			Key_Event();				//���󰴼���ֵ��ȡ					****�޸İ���*****
			key_scan();					//����������ֵ��ȡ	
			
			
			switch(Set_flag)
			{	
				case 0:		Set0_Home();		// ������ ;
					break;
				case 1:		Set1_Area();		// 1.�����趨 ;
					break;
				case 2:		Set2_Hand();		// 2.�ֶ���ժ ;
					break;
				case 3:		Set3_Auto();		// 3.�Զ���ժ ;
					break;
				case 4:		Set4_Adjust();	// 4.У׼ ;
					break;
				default:break;
			}
			
			LCD_Display_Fresh();// Һ����ʾ;
			
		}
}

