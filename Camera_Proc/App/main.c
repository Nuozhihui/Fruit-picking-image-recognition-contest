/*
 * @file          app.c
 * @brief         主函数程序
 * @ucos-ii       V2.86 
 * @function      USART1---UART5 通信    
 * @compiler      RealView MDK-ARM Compiler V4.11
 * @peripheral    V3.5.0
 * @author  	  Yuhaiyang 
 * @date   	      2015/10/26
 * @corporation   浙江求是科教设备有限公司
 * @website       http://www.zjqiushi.com/
*/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "sys.h"
#include  "bsp.h"
//#include "ov7670.h"	
#include "ov7725.h" 		 
#include "sccb.h"	
#include "STDIO.h"
#include "exti.h"
#include "delay.h"
/* Private variables ---------------------------------------------------------*/
// Systick delay
__IO uint32_t TimingDelay = 0 ;

extern u8 ov_sta;	//在exit.c里 面定义

int redThre = 55; // 115~135    
int saturationTh = 7; //55~65  
u8 FLAG_1=0;
u16 x_arr[330],y_arr[330];
u16 l,h;

u8 Send_Flag,Send_Step;//
u8 Send_Aim,Send_Full;
u16 Send_X,Send_Y; 
u16 Aim_Num; 
u16 Grid_flag=0; 


//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i;
 	u8 color1,color2;
 	u16 dadt2;
	u8 R,G,B,minValue,maxValue;//
	double HI,SI,VI;
	u16 x_mid,y_mid;
	u16 h_min,h_max,h_num;
	u16 l_min,l_max,l_num;
	
	x_mid=0;		y_mid=0;
	
	if(ov_sta)//有帧中断更新
	{
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H;
		
		for(l=0;l<240;l++)
		{
			for(h=0;h<320;h++)
			{
				OV7725_RCK_L;
				color1 = ((GPIOE->IDR&0XFF00)>>8);	//读数据
				OV7725_RCK_H; 
				OV7725_RCK_L;
				color2 = ((GPIOE->IDR&0XFF00)>>8);	//读数据
				OV7725_RCK_H; 
				
				dadt2=(color1<<8)|color2;
				
				
				R = (dadt2>>8)&0xf8;
				G = (dadt2>>3)&0xfc;
				B = dadt2<<3;
				
					if(B > G)	{minValue = G;	maxValue = B;}
					else	{minValue = B;	maxValue = G;}
					if(R >= maxValue)	maxValue = R;
					else if(R < minValue)	minValue = R;
					if(maxValue == R)	HI = (G-B)*1.0/(maxValue-minValue);
					else if(maxValue == G)	HI = 2+ (B-R)*1.0/(maxValue-minValue);
					else if(maxValue == B)	HI = 4+ (R-G)*1.0/(maxValue-minValue);
					HI = HI*60;
					if(HI <0 ) HI = HI+360;
					VI = maxValue;
					SI = (maxValue-minValue)*1.0/maxValue;
					
					//R > RT  R>=G>=B  S>=((255-R)*ST/RT)
					// H=360,红色; H=300,品红; H=60,黄色;S=0,只有灰度;S=0,V=0,黑;S=0,V=1,白;
					//  V>75% 且 S>20% 亮彩色区域;(255的比例);
					//  V>75% 且 S<20% 白色;
					//  V<25% 黑色;
					//  其他为彩色区域; V>25% && V<75% ;
					if( ((HI > 300)||(HI < 60)) && (VI>64) ) 	// &&      && (SI>51) (SI>71))||(VI<240))  && ((VI>191)&&(SI>51))
					{
						//dadt2 = 0x0000;	// 黑色;
						h_num++;
						if(h_num==1) h_min = h;
					}
					else
					{
						if(h_num!=0)
						{
							h_max = h;
							if((h_max-h_min)>50) 
							{
								for(i=h_min;i<=h_max;i++)
								x_arr[i]=1;
								y_arr[l]=1;
							}
						}
						h_num=0;
						dadt2 = 0xffff;	// 白色;
					}
		
			}
		}
		
	
		// 取中心点;(X,Y)=(y_mid,x_mid);
		h_max=0;h_min=0;
		for(h = 0; h < 320; h ++)
		{
			if(x_arr[h]!=0)		{	h_min=h;	break;	}
		}
		for(h = 319; h > 0; h --)
		{
			if(x_arr[h]!=0)		{	h_max=h;	break;	}
		}
		
		l_max=0;l_min=0;
		for(l = 0; l < 240; l ++)
		{
			if(y_arr[l]!=0)		{	l_min=l;	break;	}
		}
		for(l = 239; l > 0; l --)
		{
			if(y_arr[l]!=0)		{	l_max=l;	break;	}
		}
			
			if((h_max!=h_min) && (l_max!=l_min))
			{
					Send_Aim = 0xFB;// 有目标出现;
				
					Grid_flag=1;
					
			if(((h_max-h_min)>75) && ((l_max-l_min)>75))
			//		if((h_min<5)||(h_max>315)||(l_min<5)||(l_max>235))
					Send_Full = 0xFC;// 物体全部在画面内;
					else 
					Send_Full = 0xFD;// 物体部分在画面内;
					
					Send_X = (l_max+l_min)/2;
					Send_Y = 320-(h_max+h_min)/2;// 物体中心坐标  ;
			}
			else
			{
				//Aim_Num++;
				//if(Aim_Num>2)
				//{
				//	Aim_Num=5;
					Grid_flag=0;
					if(Send_Step==2)Send_Step=1;
					Send_Aim = 0xFA;// 没有目标出现;
				//}
			}
 		ov_sta=0;					//清零帧中断标记
	}
}


int main(void)
{
    BSP_Configuration();
	BSP_USART_Configuration(UART4);
	 delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    if (SysTick_Config(72000))		    //时钟节拍中断时1ms一次  用于定时 
    {
        while (1);
    }
		
FLAG_1=0;
		
	while(1)//初始化OV7725_OV7670
	{
		if(OV7725_Init()==0)//初始化OV7725,//返回0:成功
		{
			//sensor=OV7725;
					OV7725_Window_Set(320,240,0);//QVGA模式输出		 	   
			OV7725_Light_Mode(0);//白平衡设置;0:自动,1:太阳sunny;2,阴天cloudy;3,办公室office;4,家里home;
			OV7725_Color_Saturation(4);//色度设置;sat:-2~+2;
			OV7725_Brightness(-4);//亮度设置;//bright：-2~+2;
			OV7725_Contrast(-4);//对比度设置;//contrast：-2~+2;
			OV7725_Special_Effects(0);//特效设置;0:普通模式;1,负片;2,黑白;3,偏红色;4,偏绿色;5,偏蓝色;6,复古;
			//OV7725_Window_Set(12,176,240,320);//设置窗口
			OV7725_CS=0;
			break;
			/*
			OV7725_Light_Mode(0);//白平衡设置;0:自动,1:太阳sunny;2,阴天cloudy;3,办公室office;4,家里home;
			OV7725_Color_Saturation(4);//色度设置;sat:-4~+4;
			OV7725_Brightness(4);//亮度设置;//bright：-4~+4;
			OV7725_Contrast(4);//对比度设置;//contrast：-4~+4;
			OV7725_Special_Effects(0);//特效设置;0:普通模式;1,负片;2,黑白;3,偏红色;4,偏绿色;5,偏蓝色;6,复古;
			
			OV7725_CS=0;
			break;
			*/
		}
	}
									  
	EXTI15_10_Init();						//使能外部中断8,捕获帧中断	
	
		Send_Aim=0xFA;	
	//	Send_Flag=1;
		Send_Step=1;				 	 
	//
 	while(1)
	{
		for(h = 0; h < 320; h ++)	x_arr[h]=0;
		for(l = 0; l < 240; l ++)	y_arr[l]=0;
		
			switch(Send_Step)
			{
				case 1:	// 是否有目标出现;
					Grid_flag=0;
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, 0x80);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_Aim);
					break;
				case 2:// 物体中心坐标;// 物体部分在画面内;
					if(Grid_flag==1)
					{
						Grid_flag=0;
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, 0x80);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, 0xFE);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_Full);
				
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_X/100);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_X/10%10);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_X%10);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_Y/100);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_Y/10%10);
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, Send_Y%10);
				
		while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
        USART_SendData(UART4, 0xFE);
					}
					break;
				default:break;
			}
		//if(Send_Aim==0xFB)
	//	if(Send_Flag==1)
	//	{
		/*
			switch(Send_Aim)
			{
				case 0xFA:	// 是否有目标出现;
						Grid_flag=0;
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, 0x80);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, 0XFA);
					break;
				case 0xFB:// 物体中心坐标;// 物体部分在画面内;
					if(Grid_flag==1)
					{
						Grid_flag=0;
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, 0x80);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, 0xFB);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, Send_Full);
						
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, Send_X/100);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, Send_X/10%10);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, Send_X%10);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, Send_Y/100);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, Send_Y/10%10);
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, Send_Y%10);
						
						while(USART_GetFlagStatus(UART4, USART_FLAG_TC)==RESET);      //判断是否发送完成。
						USART_SendData(UART4, 0xFE);
					}
					break;
				default:break;
			}
	//	}

		*/

		OV7725_camera_refresh();	//更新显示
	}	

}


