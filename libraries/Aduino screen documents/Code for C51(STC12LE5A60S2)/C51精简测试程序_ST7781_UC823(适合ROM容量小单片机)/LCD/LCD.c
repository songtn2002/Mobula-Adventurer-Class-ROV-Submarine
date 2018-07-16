#include "sys.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STC12LE5A60S2,����30M  ��Ƭ��������ѹ3.3V
//QDtech-TFTҺ������ for C51
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
//********************************************************************************
//=========================================��Դ����================================================//
//5V��DC 5V��Դ
//GND�ӵ�
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ8λ����ģʽ
//8λģʽ���ߣ�
//Һ����LCD_D0��Ӧ��Ƭ��P2.0
//Һ����LCD_D1��Ӧ��Ƭ��P2.1
//Һ����LCD_D2��Ӧ��Ƭ��P2.2
//Һ����LCD_D3��Ӧ��Ƭ��P2.3
//Һ����LCD_D4��Ӧ��Ƭ��P2.4
//Һ����LCD_D5��Ӧ��Ƭ��P2.5
//Һ����LCD_D6��Ӧ��Ƭ��P2.6
//Һ����LCD_D7��Ӧ��Ƭ��P2.7
//=======================================Һ���������߽���==========================================//
//LCD_RST	��P3.3	//��λ�ź�
//LCD_CS	��P1.3	//Ƭѡ�ź�
//LCD_RS	��P1.2	//�Ĵ���/����ѡ���ź�
//LCD_WR	��P1.1	//д�ź�
//LCD_RD	��P1.0	//���ź�
//=========================================������������=========================================//
//��ģ�鴥�������������ⲿXPT2046��������оƬ�����Ա����򲻽��д������ԡ�
//��ʹ�ô�������ģ�鱾������������ɲ�����
/**************************************************************************************************/	

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;


//******************************************************************
//��������  LCD_WR_REG
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(int Reg)	 
{	
	LCD_RS=0;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8λ��������д��ʱ�� 
	LCD_DataPortH=Reg>>8;	
	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=Reg;	
#else	//16λ��������д��ʱ�� 
	LCD_DataPortH=Reg>>8;	
	LCD_DataPortL=Reg;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
} 

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�    xiao��@ȫ������
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д8λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
 void LCD_WR_DATA(int Data)
{
	LCD_RS=1;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8λ��������д��ʱ�� 
	LCD_DataPortH=Data>>8;	
	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=Data;	
#else	//16λ��������д��ʱ�� 
	LCD_DataPortH=Data>>8;	
	LCD_DataPortL=Data;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
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
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
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
	u16 i,j;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
    for(i=0;i<lcddev.width;i++)
	 {
	  for (j=0;j<lcddev.height;j++)
	   	{
        	 LCD_WR_DATA(Color);
	    }

	  }
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
	LCD_SetWindows(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(POINT_COLOR); 	    
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
void LCD_Reset(void)
{
	LCD_RESET=1;
	delay_ms(50);	
	LCD_RESET=0;
	delay_ms(50);
	LCD_RESET=1;
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
	LCD_Reset(); //��ʼ��֮ǰ��λ
	LCD_RD=1;	 //RD����û���õ������ߴ���

	
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
	

	//����LCD���Բ���
	LCD_SetParam();//����LCD����	 
	LCD_BL=1;//��������	 
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





