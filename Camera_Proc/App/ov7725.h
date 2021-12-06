#ifndef _OV7725_H
#define _OV7725_H
#include "sys.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////
//ALIENTEK��ӢSTM32������
//OV7725 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2017/11/1
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////


#define OV7725_MID				0X7FA2    
#define OV7725_PID				0X7721

#define OV7725_VSYNC  	PFin(13)			//ͬ���źż��IO
#define OV7725_WRST		PFout(14)		//дָ�븴λ
#define OV7725_WREN		PBout(11)		//д��FIFOʹ��
#define OV7725_RCK_H	GPIOB->BSRR=1<<10//���ö�����ʱ�Ӹߵ�ƽ
#define OV7725_RCK_L	GPIOB->BRR=1<<10	//���ö�����ʱ�ӵ͵�ƽ
#define OV7725_RRST		PGout(0) 		//��ָ�븴λ
#define OV7725_CS		PEout(7)  		//Ƭѡ�ź�(OE)
								  				
#define OV7725_DATA   	GPIO_ReadInputData(GPIOE,0xFF00) 	//��������˿�



	    				 
u8   OV7725_Init(void);		  	   		 
void OV7725_Light_Mode(u8 mode);
void OV7725_Color_Saturation(s8 sat);
void OV7725_Brightness(s8 bright);
void OV7725_Contrast(s8 contrast);
void OV7725_Special_Effects(u8 eft);
void OV7725_Window_Set(u16 width,u16 height,u8 mode);
#endif




















