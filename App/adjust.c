/*
 * @file          adjust.c
 * @brief         �Զ���ժ�������ó���
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
#include "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "bsp.h"
#include  "motor.h"
#include  "keypad.h"
#include  "adjust.h"
/* Private variables ---------------------------------------------------------*/

/**********************************************************************************************************
* Function    ��void Flash_Init(void)
* Description : Flash-EEPROM��ʼ��
* Para        : none
* Return(s)   : none
* Example     : Flash_Init();
**********************************************************************************************************/
void Flash_Init(void)
{
	if((*(uint16_t*)(0x807f000)) == 0x01)// ��ѯУ׼���Ƿ��Ѵ���EEPROM,δ������ȡ�̶���;
	{
		shift_X = *(uint16_t*)(0x807f002);	shift_Y = *(uint16_t*)(0x807f004);
		MOT_Spwm_MAX = (*(uint16_t*)(0x807f006))*256 + (*(uint16_t*)(0x807f008));
		MOT_Spwm_MIN = (*(uint16_t*)(0x807f00a))*256 + (*(uint16_t*)(0x807f00c));
		Pick_time_M = *(uint16_t*)(0x807f00e);
	}
	else
	{
		shift_X = 40;		shift_Y = 50;
		MOT_Spwm_MAX = 1600;
		MOT_Spwm_MIN = 1400;
		Pick_time_M = 14;
		Flash_eepromIn();//EEPROM ģ�� FLASH ����;
	}
}

/**********************************************************************************************************
* Function    ��void Flash_eepromIn(void)
* Description : EEPROM ģ�� FLASH ���� 
* Para        : none
* Return(s)   : none
* Example     : Flash_eepromIn();
**********************************************************************************************************/
void Flash_eepromIn(void)
{
	FLASH_Unlock();	//FLASH����
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);// Clear All pending flags//���־λ
	FLASH_ErasePage(0x0807f000);// ����ҳ   512k(2Kbye):245ҳ:0x0807 f000--0x0807 f7ff;255ҳ:0x0807 f800--0x0807 ffff;
	FLASH_ProgramHalfWord(0x807f000, 0x01);
	FLASH_ProgramHalfWord(0x807f002, shift_X);				FLASH_ProgramHalfWord(0x807f004, shift_Y);
	FLASH_ProgramHalfWord(0x807f006, MOT_Spwm_MAX/256);				FLASH_ProgramHalfWord(0x807f008, MOT_Spwm_MAX%256);
	FLASH_ProgramHalfWord(0x807f00a, MOT_Spwm_MIN/256);				FLASH_ProgramHalfWord(0x807f00c, MOT_Spwm_MIN%256);
	FLASH_ProgramHalfWord(0x807f00e, Pick_time_M);
	FLASH_Lock();//FLASH���� 
}

/**********************************************************************************************************
* Function    ��void Set_flag04(void)
* Description : У׼��������
* Para        : none
* Return(s)   : none
* Example     : Set_flag04();
//MOT_Spwm_MAX 	��е�ֶ�����������;max;1500~1750�ɵ�; �� 10 ����;
//MOT_Spwm_MIN 	��е�ֶ��������С���;min;1250~1500�ɵ�; �� 10 ����;
//Pick_time_M 	Z���ȥ����������ʱ;
//shift_X,shift_Y �Զ���ժʱ X,Y������ͷƫ����;X:0~80mm�ɵ�;Y:0~100mm�ɵ�;�� 1 ����;
**********************************************************************************************************/
void Set4_Adjust(void)
{
		unsigned int Sure=0;
	
		switch(key_val)		//���󰴼�ֵ
		{
				case 9:	if(Set_Wei>0)Set_Wei=Set_Wei-1;			break;	// ����;
				case 13:Set_Wei=(Set_Wei+1)%6;							break;	// ����;
			
				case 1:	Sure = 0;	// ����;
					switch(Set_Wei)
					{
						// 	��е�ֶ�����������;max;1500~1750�ɵ�;
						case 1:	if(MOT_Spwm_MAX<1750)	MOT_Spwm_MAX = MOT_Spwm_MAX + 10;
							break;
						// 	��е�ֶ��������С���;min;1250~1500�ɵ�;
						case 2:	if(MOT_Spwm_MIN<1500)	MOT_Spwm_MIN = MOT_Spwm_MIN + 10;
							break;
						
						// 	Z���ȥ����������ʱ;
						case 3:	Pick_time_M =  Pick_time_M + 1;
							break;
						
						// �Զ���ժʱ X,Y������ͷƫ����;X:0~80�ɵ�;Y:0~100�ɵ�;
						case 4:	if(shift_X<80)	shift_X = shift_X + 1;
							break;
						
						// �Զ���ժʱ X,Y������ͷƫ����;X:0~80�ɵ�;Y:0~100�ɵ�;
						case 5:	if(shift_Y<100)	shift_Y = shift_Y + 1;
							break;
						default:	break;
					}
					break;
					
				case 5:	Sure = 0;	// ����;
					switch(Set_Wei)
					{
						case 1: if(MOT_Spwm_MAX>1500)	MOT_Spwm_MAX = MOT_Spwm_MAX - 10;
							break;
						case 2:	if(MOT_Spwm_MIN>1250)	MOT_Spwm_MIN = MOT_Spwm_MIN - 10;
							break;
						case 3:	if(Pick_time_M>0)	Pick_time_M =  Pick_time_M - 1;
							break;
						case 4:	if(shift_X>0)	shift_X = shift_X - 1;
							break;
						case 5:	if(shift_Y>0)	shift_Y = shift_Y - 1;
							break;
						default:	break;
					}
					break;
				case 16:	Sure=1;	Flash_eepromIn();//EEPROM ģ�� FLASH ����;
					break;// E ;
				default:	break;
		}
		key_val=0;
		
		if(Sure==1) // ���沢�˻������� ;
		{
			Sure = 0;
			Set_flag=0;LCD_flag=1;Set_Wei=0;
			AuRun_Flag=0;// ����޲���;
			TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//��ֹʹ��ʱ��TIM2��ʱ�ж�Դ�ж�
			TIM_Cmd(TIM2, DISABLE);											//��ֹʹ�ܶ�ʱ��2
			
		}
		
		//ǿ�Ʒ���
		if(press_flag1==1)// ֱ���˻������� ;
		{
				shift_X = *(uint16_t*)(0x807f002);	shift_Y = *(uint16_t*)(0x807f004);
				MOT_Spwm_MAX = (*(uint16_t*)(0x807f006))*256 + (*(uint16_t*)(0x807f008));
				MOT_Spwm_MIN = (*(uint16_t*)(0x807f00a))*256 + (*(uint16_t*)(0x807f00c));
				Pick_time_M = *(uint16_t*)(0x807f00e);
			Sure = 0;
			Set_flag=0;LCD_flag=1;Set_Wei=0;
			AuRun_Flag=0;// ����޲���;
			TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//��ֹʹ��ʱ��TIM2��ʱ�ж�Դ�ж�
			TIM_Cmd(TIM2, DISABLE);											//��ֹʹ�ܶ�ʱ��2
		}
		
		//****************************************************************************************************//
		
				if((press_flag4==1)&&(Auto_UART==0)&&(Set_Wei==0))// �� SW4 ����Զ�ץ��  &&(AuRun_Flag==2)
				{
					//key_val=0;
					AuRun_Flag=2;			// ����ֶ�ɨ��;
					Auto_UART=1;			// ����ͨѶ����;
					AuIma_ALL=0;// �ȴ��ж��Ƿ�������;
				}
				
				// �������:1-����λ(�ϵ��һ��);2-�ֶ�����;3-���������;4-�Զ�����;5-��׷��ǰλ��;6-׷��Ŀ��;
				if(AuRun_Flag == 0)	AuRun_Flag=2;			// ����ֶ�ɨ��;
				
				
				if((press_flag3==1)&&(Set_Wei==0))	Apple_Pick=1;// ������ժ;
				
				if(Apple_Pick!=0)															// ��ժ��;
				{
					switch(Apple_Pick)
					{
						case 1:	// ȷ�ϲ�ժ;// ����00;
							
										// �ֶ���ժ��е��ǰ��,���˼�ʱ;
										Pick_time1=0;
						
										// X,Y,Z.�������: 0,ֹͣ; 1,����,����,ǰ��; 2,����,����,����;,MOT_DirV
										MOT_DirZ=0;
						
										// ��е�ֶ��PWM;
										MOT_Spwm=MOT_Spwm_M;
						
										// ˮ����ժ�����ּ�;0-�޶���;1-��ʼ����;2,3,4,....ֱ������������0;
										Apple_Pick=2;
						
						
										AuRun_Flag=0;	// ��ֹ�������;
										break;
						case 2:	// ����02;
										Pick_time1++;			MOT_DirZ=1;// Z����ǰ��;
										if(MOT_Spwm<MOT_Spwm_MAX)	MOT_Spwm=MOT_Spwm+10;
										if(Pick_time1>(Pick_time_M*5))		{	Pick_time1=0;	Apple_Pick=3;	}
										break;
						case 3:	// ����03;
										Pick_time1++;			MOT_DirZ=0;
										if((MOT_Spwm>MOT_Spwm_MIN))	MOT_Spwm=MOT_Spwm-10;//&&(AD_RunV_over!=1)
										if(Pick_time1>30)	{	Pick_time1=0; Apple_Pick=4;}
										break;
						case 4:		// ����04;
										Pick_time1++;			MOT_DirZ=2;// ��������;
										if(Pick_time1>(Pick_time_M*5))	{AuRun_Flag=2;Pick_time1=0;Apple_Pick=5;}
										break;
						case 5:		// ����05;
										Pick_time1++;			MOT_DirZ=0;
										if(MOT_Spwm<MOT_Spwm_MAX)	MOT_Spwm=MOT_Spwm+10;
										if(Pick_time1>30){Pick_time1=0;Apple_Pick=6;}// ��ʵ����;Run_Num++;Alarm_Ctrl();
										break;
						case 6:		// ������ժ;��0;
										Pick_time1++;
										if(MOT_Spwm>MOT_Spwm_M)	MOT_Spwm=MOT_Spwm-10;
										if(Pick_time1>30)
										{
											Pick_time1=0;
											MOT_DirZ=0;
											MOT_Spwm=MOT_Spwm_M;
											Apple_Pick=0;
											AuRun_Flag=2;
											//AD_RunV_over=0;
										}
										break;
						default:break;
					}
					if(MOT_DirZ==0) MOT_Zpwm = 10000;				// ֹͣ;
					else if(MOT_DirZ==1) MOT_Zpwm = 0;			// ǰ��;
					else if(MOT_DirZ==2) MOT_Zpwm = 20000;	// ����;
					TIM_SetCompare3(TIM4,MOT_Zpwm);					// TIM4_CRR3-Z;
					TIM_SetCompare4(TIM4,MOT_Spwm);					// TIM4_CRR4;
				}
				MOTOR_Set();				// �������;
		
}







