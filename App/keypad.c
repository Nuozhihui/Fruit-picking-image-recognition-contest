/*
 * keypad.c
 *
 *  Created on: 2013-3-19
 *      Author: think
 */

#include  "stm32f10x.h"
#include  "stm32f10x_conf.h" 
#include  "bsp.h"
#include  "LCD12864.h"
#include  "keypad.h"

/* 4X4���󰴼�
  * P7,P2���Ǹ�4λ������P7�����P2����
 * */

/*
��������ο���
1:��
2:	7
3:	8
4:	9
5:��
6:	4
7:	5
8:	6
9����
10��1(�����趨)
11:	2
12��3
13:	��	
14:	D
15:	O
16:E����
*/


unsigned char	key_Map[] = {10,11,12,6,7,8,2,3,4,15,16,14,1,5,9,13}; //16�������ļ�ֵ����		****�޸İ���*****

void Check_Key(void)
{
	unsigned char row, col; 
	unsigned int KEY_DOUT,tmp1, tmp2;
	tmp1 = 0x0800;
	for(row=0; row<4; row++)		//��ɨ��
	{	
		KEY_DOUT = 0X0f00;		//���ȫΪ1
		KEY_DOUT-= tmp1;		//�������һ��Ϊ0
		GPIOD->ODR=((GPIOD->ODR&0xf0ff)|KEY_DOUT);
		tmp1 >>=1;
		if((GPIO_ReadInputData(GPIOD)&0xf000)<0xf000)
		//if((KEY_DIN & 0xF0) < 0xF0) //P2�����Ƿ���һλΪ0
		{
			tmp2 = 0x1000;			//���ڼ�����һλΪ0
			for(col=0; col<4; col++)	//��ɨ��
			{
				if(0x00 == (GPIO_ReadInputData(GPIOD) & tmp2))	//�ҵ�����0����
				{
					key_val = key_Map[row*4 + col];//��ȡ��ֵ
					return;		//�˳�ѭ��
				}
				tmp2 <<= 1;		//����1λ
			}
		}

	}
}


void Key_Event(void)
{
	unsigned int tmp;
	GPIOD->ODR=((GPIOD->ODR&0xf0ff)|0x0000);
	
	tmp = GPIO_ReadInputData(GPIOD);			
	if ((0x00 == key_Pressed) && ((tmp & 0xF000) < 0xF000))		//����м�����
	{
		key_Pressed = 1;		//�������±�ʶλ��λ
		Delay(10);				//��ʱȥ��
		Check_Key();			//��ȡ��
//		key_flag    = 1;		//������ʶ��λ
	}
    else if ((key_Pressed == 1)&&((tmp & 0xf000) == 0xF000))		//��������ͷ�		// 0xf000��ʮ����=  1111000000000000(��)  ��������ΪGPIO D�ĵ�ƽ ��ԭ��ͼ-ֻʹ���˺�4��IO��
    {
			
        key_Pressed = 0;		//�����ʶλ
        key_flag    = 1;		//������ʶλ��λ
		
    }
    else
    {
       Delay(1);
    }
}







