/*
 * @file          adjust.c
 * @brief         自动采摘参数设置程序
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
#include "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "bsp.h"
#include  "motor.h"
#include  "keypad.h"
#include  "adjust.h"
/* Private variables ---------------------------------------------------------*/

/**********************************************************************************************************
* Function    ：void Flash_Init(void)
* Description : Flash-EEPROM初始化
* Para        : none
* Return(s)   : none
* Example     : Flash_Init();
**********************************************************************************************************/
void Flash_Init(void)
{
	if((*(uint16_t*)(0x807f000)) == 0x01)// 查询校准点是否已存入EEPROM,未存入则取固定点;
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
		Flash_eepromIn();//EEPROM 模拟 FLASH 存入;
	}
}

/**********************************************************************************************************
* Function    ：void Flash_eepromIn(void)
* Description : EEPROM 模拟 FLASH 存入 
* Para        : none
* Return(s)   : none
* Example     : Flash_eepromIn();
**********************************************************************************************************/
void Flash_eepromIn(void)
{
	FLASH_Unlock();	//FLASH解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);// Clear All pending flags//清标志位
	FLASH_ErasePage(0x0807f000);// 擦除页   512k(2Kbye):245页:0x0807 f000--0x0807 f7ff;255页:0x0807 f800--0x0807 ffff;
	FLASH_ProgramHalfWord(0x807f000, 0x01);
	FLASH_ProgramHalfWord(0x807f002, shift_X);				FLASH_ProgramHalfWord(0x807f004, shift_Y);
	FLASH_ProgramHalfWord(0x807f006, MOT_Spwm_MAX/256);				FLASH_ProgramHalfWord(0x807f008, MOT_Spwm_MAX%256);
	FLASH_ProgramHalfWord(0x807f00a, MOT_Spwm_MIN/256);				FLASH_ProgramHalfWord(0x807f00c, MOT_Spwm_MIN%256);
	FLASH_ProgramHalfWord(0x807f00e, Pick_time_M);
	FLASH_Lock();//FLASH锁定 
}

/**********************************************************************************************************
* Function    ：void Set_flag04(void)
* Description : 校准参数程序
* Para        : none
* Return(s)   : none
* Example     : Set_flag04();
//MOT_Spwm_MAX 	机械手舵机伸缩最大间距;max;1500~1750可调; 按 10 增减;
//MOT_Spwm_MIN 	机械手舵机伸缩最小间距;min;1250~1500可调; 按 10 增减;
//Pick_time_M 	Z轴出去或回来最大延时;
//shift_X,shift_Y 自动采摘时 X,Y轴摄像头偏移量;X:0~80mm可调;Y:0~100mm可调;按 1 增减;
**********************************************************************************************************/
void Set4_Adjust(void)
{
		unsigned int Sure=0;
	
		switch(key_val)		//矩阵按键值
		{
				case 9:	if(Set_Wei>0)Set_Wei=Set_Wei-1;			break;	// 向左;
				case 13:Set_Wei=(Set_Wei+1)%6;							break;	// 向右;
			
				case 1:	Sure = 0;	// 向上;
					switch(Set_Wei)
					{
						// 	机械手舵机伸缩最大间距;max;1500~1750可调;
						case 1:	if(MOT_Spwm_MAX<1750)	MOT_Spwm_MAX = MOT_Spwm_MAX + 10;
							break;
						// 	机械手舵机伸缩最小间距;min;1250~1500可调;
						case 2:	if(MOT_Spwm_MIN<1500)	MOT_Spwm_MIN = MOT_Spwm_MIN + 10;
							break;
						
						// 	Z轴出去或回来最大延时;
						case 3:	Pick_time_M =  Pick_time_M + 1;
							break;
						
						// 自动采摘时 X,Y轴摄像头偏移量;X:0~80可调;Y:0~100可调;
						case 4:	if(shift_X<80)	shift_X = shift_X + 1;
							break;
						
						// 自动采摘时 X,Y轴摄像头偏移量;X:0~80可调;Y:0~100可调;
						case 5:	if(shift_Y<100)	shift_Y = shift_Y + 1;
							break;
						default:	break;
					}
					break;
					
				case 5:	Sure = 0;	// 向下;
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
				case 16:	Sure=1;	Flash_eepromIn();//EEPROM 模拟 FLASH 存入;
					break;// E ;
				default:	break;
		}
		key_val=0;
		
		if(Sure==1) // 保存并退回主界面 ;
		{
			Sure = 0;
			Set_flag=0;LCD_flag=1;Set_Wei=0;
			AuRun_Flag=0;// 电机无操作;
			TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//禁止使能时钟TIM2定时中断源中断
			TIM_Cmd(TIM2, DISABLE);											//禁止使能定时器2
			
		}
		
		//强制返回
		if(press_flag1==1)// 直接退回主界面 ;
		{
				shift_X = *(uint16_t*)(0x807f002);	shift_Y = *(uint16_t*)(0x807f004);
				MOT_Spwm_MAX = (*(uint16_t*)(0x807f006))*256 + (*(uint16_t*)(0x807f008));
				MOT_Spwm_MIN = (*(uint16_t*)(0x807f00a))*256 + (*(uint16_t*)(0x807f00c));
				Pick_time_M = *(uint16_t*)(0x807f00e);
			Sure = 0;
			Set_flag=0;LCD_flag=1;Set_Wei=0;
			AuRun_Flag=0;// 电机无操作;
			TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//禁止使能时钟TIM2定时中断源中断
			TIM_Cmd(TIM2, DISABLE);											//禁止使能定时器2
		}
		
		//****************************************************************************************************//
		
				if((press_flag4==1)&&(Auto_UART==0)&&(Set_Wei==0))// 按 SW4 电机自动抓点  &&(AuRun_Flag==2)
				{
					//key_val=0;
					AuRun_Flag=2;			// 电机手动扫描;
					Auto_UART=1;			// 启动通讯接收;
					AuIma_ALL=0;// 等待判定是否在屏内;
				}
				
				// 电机动作:1-归零位(上电仅一次);2-手动操作;3-归区域零点;4-自动运行;5-归追踪前位置;6-追踪目标;
				if(AuRun_Flag == 0)	AuRun_Flag=2;			// 电机手动扫描;
				
				
				if((press_flag3==1)&&(Set_Wei==0))	Apple_Pick=1;// 启动采摘;
				
				if(Apple_Pick!=0)															// 采摘中;
				{
					switch(Apple_Pick)
					{
						case 1:	// 确认采摘;// 正常00;
							
										// 手动采摘机械手前进,后退计时;
										Pick_time1=0;
						
										// X,Y,Z.电机方向: 0,停止; 1,右移,上移,前进; 2,左移,下移,后退;,MOT_DirV
										MOT_DirZ=0;
						
										// 机械手舵机PWM;
										MOT_Spwm=MOT_Spwm_M;
						
										// 水果采摘动作分级;0-无动作;1-开始动作;2,3,4,....直至动作结束归0;
										Apple_Pick=2;
						
						
										AuRun_Flag=0;	// 禁止电机操作;
										break;
						case 2:	// 正常02;
										Pick_time1++;			MOT_DirZ=1;// Z轴电机前进;
										if(MOT_Spwm<MOT_Spwm_MAX)	MOT_Spwm=MOT_Spwm+10;
										if(Pick_time1>(Pick_time_M*5))		{	Pick_time1=0;	Apple_Pick=3;	}
										break;
						case 3:	// 正常03;
										Pick_time1++;			MOT_DirZ=0;
										if((MOT_Spwm>MOT_Spwm_MIN))	MOT_Spwm=MOT_Spwm-10;//&&(AD_RunV_over!=1)
										if(Pick_time1>30)	{	Pick_time1=0; Apple_Pick=4;}
										break;
						case 4:		// 正常04;
										Pick_time1++;			MOT_DirZ=2;// 正常后退;
										if(Pick_time1>(Pick_time_M*5))	{AuRun_Flag=2;Pick_time1=0;Apple_Pick=5;}
										break;
						case 5:		// 正常05;
										Pick_time1++;			MOT_DirZ=0;
										if(MOT_Spwm<MOT_Spwm_MAX)	MOT_Spwm=MOT_Spwm+10;
										if(Pick_time1>30){Pick_time1=0;Apple_Pick=6;}// 果实计数;Run_Num++;Alarm_Ctrl();
										break;
						case 6:		// 结束采摘;归0;
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
					if(MOT_DirZ==0) MOT_Zpwm = 10000;				// 停止;
					else if(MOT_DirZ==1) MOT_Zpwm = 0;			// 前进;
					else if(MOT_DirZ==2) MOT_Zpwm = 20000;	// 后退;
					TIM_SetCompare3(TIM4,MOT_Zpwm);					// TIM4_CRR3-Z;
					TIM_SetCompare4(TIM4,MOT_Spwm);					// TIM4_CRR4;
				}
				MOTOR_Set();				// 电机运行;
		
}








