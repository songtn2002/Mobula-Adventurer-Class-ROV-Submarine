#include "lcd.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103RBT6,��Ƶ72M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for STM32 IOģ��
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtech.net
//�Ա���վ��http://qdtech.taobao.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:QDtech2008@gmail.com 
//Skype:QDtech2008
//��������QQȺ:324828016
//��������:2013/5/13
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////	
/****************************************************************************************************
//=========================================��Դ����================================================//
//5V��DC 5V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ8λ����ģʽ
//8λģʽ���ߣ���ģ��ӵ�8λ���ߣ���:
//Һ����LCD_D0��Ӧ��Ƭ��PB0
//Һ����LCD_D1��Ӧ��Ƭ��PB1
//Һ����LCD_D2��Ӧ��Ƭ��PB2
//Һ����LCD_D3��Ӧ��Ƭ��PB3
//Һ����LCD_D4��Ӧ��Ƭ��PB4
//Һ����LCD_D5��Ӧ��Ƭ��PB5
//Һ����LCD_D6��Ӧ��Ƭ��PB6
//Һ����LCD_D7��Ӧ��Ƭ��PB7
//=======================================Һ���������߽���==========================================//
//LCD_RST	��PC5	//��λ�ź�
//LCD_CS	��PC9	//Ƭѡ�ź�
//LCD_RS	��PC8	//�Ĵ���/����ѡ���ź�
//LCD_WR	��PC7	//д�ź�
//LCD_RD	��PC6	//���ź�
//=========================================������������=========================================//
//��ģ�鴥�������������ⲿXPT2046��������оƬ�����Ա����򲻽��д������ԡ�
//��ʹ�ô�������ģ�鱾������������ɲ�����
**************************************************************************************************/	

	   
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


//******************************************************************
//��������  LCD_WR_REG
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(u8 data)
{ 
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
	LCD_RS_CLR;//д��ַ  
	LCD_CS_CLR;
	
	
	DATAOUT(data>>8);	//����8λ���ݴ���������PB0~PB7���ͳ�ȥ
	LCD_WR_CLR;
	LCD_WR_SET;
		
	DATAOUT(data);//����8λ���ݴ���������PB0~PB7���ͳ�ȥ
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
#else//ʹ��16λ������������ģʽ
	LCD_RS_SET;
	LCD_CS_CLR;
	LCD_WR_CLR;
	DATAOUT(data);		
	LCD_WR_SET;
	LCD_CS_SET;
			
#endif
}

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_DATA(u16 data)
{
#if LCD_USE8BIT_MODEL==1//ʹ��8λ������������ģʽ
		LCD_RS_SET;
		LCD_CS_CLR;
		
		DATAOUT(data>>8);	//����8λ���ݴ���������PB0~PB7���ͳ�ȥ	
		LCD_WR_CLR;
		LCD_WR_SET;
		
		DATAOUT(data);	//����8λ���ݴ���������PB0~PB7���ͳ�ȥ	
		LCD_WR_CLR;
		LCD_WR_SET;
	
		LCD_CS_SET;
		
#else//ʹ��16λ������������ģʽ
		LCD_RS_SET;
		LCD_CS_CLR;
		LCD_WR_CLR;
		DATAOUT(data);		
		LCD_WR_SET;
		LCD_CS_SET;
			
#endif
}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
}	   
	 
//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
}	 

//******************************************************************
//��������  LCD_DrawPoint
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��ָ��λ��д��һ�����ص�����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATA(POINT_COLOR); 
}

//******************************************************************
//��������  LCD_Clear
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCDȫ�������������
//���������Color:Ҫ���������ɫ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	  
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_WR_DATA(Color);    
	}
} 

//******************************************************************
//��������  LCD_GPIOInit
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    Һ����IO��ʼ����Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5;	   //GPIO_Pin_10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOC	
	GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	//  
	GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_All);	 
}

//******************************************************************
//��������  LCD_Reset
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��λ������Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay_ms(100);	
	LCD_RST_SET;
	delay_ms(50);
}
 	 
//******************************************************************
//��������  LCD_Init
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{  
     										 
	LCD_GPIOInit();
 	LCD_RESET();
	LCD_RD_SET;
	
	//��ʼ��
	LCD_WriteReg(0x0000,0x0001);
	delay_ms(100);  //at least 100ms
	LCD_WriteReg(0x0001,0x0000); // set SS and SM bit
	LCD_WriteReg(0x0010,0x1790);
	LCD_WriteReg(0x0060,0x2700);
	LCD_WriteReg(0x0061,0x0001);
	LCD_WriteReg(0x0046,0x0002);
	LCD_WriteReg(0x0013,0x8010);
	LCD_WriteReg(0x0012,0x80fe);
	LCD_WriteReg(0x0002,0x0500);
	LCD_WriteReg(0x0003,0x1030);

	LCD_WriteReg(0x0030,0x0303);
	LCD_WriteReg(0x0031,0x0303);
	LCD_WriteReg(0x0032,0x0303);
	LCD_WriteReg(0x0033,0x0300);
	LCD_WriteReg(0x0034,0x0003);
	LCD_WriteReg(0x0035,0x0303);
	LCD_WriteReg(0x0036,0x0014);
	LCD_WriteReg(0x0037,0x0303);
	LCD_WriteReg(0x0038,0x0303);
	LCD_WriteReg(0x0039,0x0303);
	LCD_WriteReg(0x003a,0x0300);
	LCD_WriteReg(0x003b,0x0003);
	LCD_WriteReg(0x003c,0x0303);
	LCD_WriteReg(0x003d,0x1400);

	LCD_WriteReg(0x0092,0x0200);
	LCD_WriteReg(0x0093,0x0303);
	LCD_WriteReg(0x0090,0x080d); 


	LCD_WriteReg(0x0001, 0x0000); // set SS and SM bit 0100
	LCD_WriteReg(0x0002, 0x0700); // set 1 line inversion
	LCD_WriteReg(0x0003, 0x1090); // set GRAM write direction and BGR=1.
	LCD_WriteReg(0x0004, 0x0000); // Resize register
	LCD_WriteReg(0x0008, 0x0302); // set the back porch and front porch
	LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
	LCD_WriteReg(0x000A, 0x0000); // FMARK function
	LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
	LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
	LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
	delay_ms(120);

	LCD_WriteReg(0x0030,0x0303);
	LCD_WriteReg(0x0031,0x0303);
	LCD_WriteReg(0x0032,0x0303);
	LCD_WriteReg(0x0033,0x0300);
	LCD_WriteReg(0x0034,0x0003);
	LCD_WriteReg(0x0035,0x0303);
	LCD_WriteReg(0x0036,0x0014);
	LCD_WriteReg(0x0037,0x0303);
	LCD_WriteReg(0x0038,0x0303);
	LCD_WriteReg(0x0039,0x0303);
	LCD_WriteReg(0x003a,0x0300);
	LCD_WriteReg(0x003b,0x0003);
	LCD_WriteReg(0x003c,0x0303);
	LCD_WriteReg(0x003d,0x1400);

	LCD_WriteReg(0x0060, 0xA700); // Gate Scan 
	LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV
	LCD_WriteReg(0x006A, 0x0000); // set scrolling line
	LCD_WriteReg(0x0090, 0x0033);
	LCD_WriteReg(0x0095, 0x0110);

	LCD_WriteReg(0x00FF, 0x0001);
	LCD_WriteReg(0x00FF, 0x000C);
	LCD_WriteReg(0x00FF, 0x0000);
	delay_ms(100);
	LCD_WriteReg(0x0003,0x1018); // set GRAM write direction and BGR=1.
	LCD_WriteReg(0x0007,0x0173);

	delay_ms(50); 
	
	LCD_SetParam();//����LCD����	 
	//LCD_setRotation(0);//������ʾ����
	LCD_LED=1;//��������	 
	LCD_Clear(WHITE);
}
  		  
/*************************************************
��������LCD_SetWindows
���ܣ�����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	if(lcddev.dir==1)//����
	{	
	LCD_WR_REG(0x52);	
	LCD_WR_DATA(xStar);
	LCD_WR_REG(0x53);		
	LCD_WR_DATA(xEnd);
	LCD_WR_REG(0x50);	
	LCD_WR_DATA(yStar);
	LCD_WR_REG(0x51);		
	LCD_WR_DATA(yEnd);
	}
	else 
	{
	LCD_WR_REG(0x50);	
	LCD_WR_DATA(xStar);
	LCD_WR_REG(0x51);		
	LCD_WR_DATA(xEnd);
	LCD_WR_REG(0x52);	
	LCD_WR_DATA(yStar);
	LCD_WR_REG(0x53);		
	LCD_WR_DATA(yEnd);
	}
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar);	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM				
}   

/*************************************************
��������LCD_SetCursor
���ܣ����ù��λ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos);

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos);

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM	
} 

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x22;
#if USE_HORIZONTAL==1	//ʹ�ú���	  
	lcddev.dir=1;//����
	lcddev.width=320;
	lcddev.height=240;
	lcddev.setxcmd=0x21;
	lcddev.setycmd=0x20;			
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0xA700);
	
		//����1���½�Ϊԭ��
  /*LCD_WriteReg(0x01,0x0000);
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0xA700);*/
	//����2���Ͻ�Ϊԭ��	
	LCD_WriteReg(0x01,0x0100); 
	LCD_WriteReg(0x03,0x0038);
	LCD_WriteReg(0x0060,0x2700);

#else//����
	lcddev.dir=0;//����				 	 		
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x20;
	lcddev.setycmd=0x21;	
	//����1���½�Ϊԭ��
	LCD_WriteReg(0x01,0x0000);
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x0060,0x2700);
	//����2���Ͻ�Ϊԭ��
	/*
	LCD_WriteReg(0x01,0x0100); 
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x0060,0xA700);*/
#endif
}	  

void LCD_setRotation(u8 rotation )
{ 
	lcddev.wramcmd=0x22;
	switch(rotation){
		case 0:			
			lcddev.dir=0;				
			lcddev.width=240;
			lcddev.height=320;	
			lcddev.setxcmd=0x20;
			lcddev.setycmd=0x21;
			LCD_WriteReg(0x01,0x0000);
			LCD_WriteReg(0x03,0x1030);
			LCD_WriteReg(0x0060,0x2700);
		break;
		
		case 1:
			lcddev.dir=1;
			lcddev.width=320;
			lcddev.height=240;
			lcddev.setxcmd=0x21;
			lcddev.setycmd=0x20;	
			LCD_WriteReg(0x01,0x0000);
			LCD_WriteReg(0x03,0x1038);
			LCD_WriteReg(0x0060,0xA700);
		break;
		
		case 2:		
			lcddev.dir=0;			
			lcddev.width=240;
			lcddev.height=320;	
			lcddev.setxcmd=0x20;
			lcddev.setycmd=0x21;
			LCD_WriteReg(0x01,0x0100); 
			LCD_WriteReg(0x03,0x0030);
			LCD_WriteReg(0x0060,0xA700);
		break;
		
		case 3:
			lcddev.dir=1;
			lcddev.width=320;
			lcddev.height=240;
			lcddev.setxcmd=0x21;
			lcddev.setycmd=0x20;	
			LCD_WriteReg(0x01,0x0100); 
			LCD_WriteReg(0x03,0x0038);
			LCD_WriteReg(0x0060,0x2700);
		break;
		
		default:break;
	}
		

}	  

