#include "sys.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STC12LE5A60S2,晶振30M  单片机工作电压3.3V
//QDtech-TFT液晶驱动 for C51
//xiao冯@ShenZhen QDtech co.,LTD
//公司网站:www.qdtech.net
//淘宝网站：http://qdtech.taobao.com
//我司提供技术支持，任何技术问题欢迎随时交流学习
//固话(传真) :+86 0755-23594567 
//手机:15989313508（冯工） 
//邮箱:QDtech2008@gmail.com 
//Skype:QDtech2008
//技术交流QQ群:324828016
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市全动电子技术有限公司 2009-2019
//All rights reserved
//********************************************************************************
//=========================================电源接线================================================//
//5V接DC 5V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为8位并行模式
//8位模式接线：
//液晶屏LCD_D0对应单片机P2.0
//液晶屏LCD_D1对应单片机P2.1
//液晶屏LCD_D2对应单片机P2.2
//液晶屏LCD_D3对应单片机P2.3
//液晶屏LCD_D4对应单片机P2.4
//液晶屏LCD_D5对应单片机P2.5
//液晶屏LCD_D6对应单片机P2.6
//液晶屏LCD_D7对应单片机P2.7
//=======================================液晶屏控制线接线==========================================//
//LCD_RST	接P3.3	//复位信号
//LCD_CS	接P1.3	//片选信号
//LCD_RS	接P1.2	//寄存器/数据选择信号
//LCD_WR	接P1.1	//写信号
//LCD_RD	接P1.0	//读信号
//=========================================触摸屏触接线=========================================//
//本模块触摸测试需连接外部XPT2046触摸采样芯片，所以本程序不进行触摸测试。
//不使用触摸或者模块本身不带触摸，则可不连接
/**************************************************************************************************/	

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;


//******************************************************************
//函数名：  LCD_WR_REG
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位指令
//输入参数：Reg:待写入的指令值
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_REG(int Reg)	 
{	
	LCD_RS=0;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8位并行数据写入时序 
	LCD_DataPortH=Reg>>8;	
	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=Reg;	
#else	//16位并行数据写入时序 
	LCD_DataPortH=Reg>>8;	
	LCD_DataPortL=Reg;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
} 

//******************************************************************
//函数名：  LCD_WR_DATA
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写8位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
 void LCD_WR_DATA(int Data)
{
	LCD_RS=1;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8位并行数据写入时序 
	LCD_DataPortH=Data>>8;	
	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=Data;	
#else	//16位并行数据写入时序 
	LCD_DataPortH=Data>>8;	
	LCD_DataPortL=Data;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
}


//******************************************************************
//函数名：  LCD_WriteReg
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}


//******************************************************************
//函数名：  LCD_Clear
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
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
//函数名：  LCD_DrawPoint
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetWindows(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 

//******************************************************************
//函数名：  LCD_Reset
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
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
//函数名：  LCD_Init
//作者：    xiao冯@全动电子
//日期：    2013-02-22
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{
	LCD_Reset(); //初始化之前复位
	LCD_RD=1;	 //RD引脚没有用到，拉高处理

	
	//初始化
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
	

	//设置LCD属性参数
	LCD_SetParam();//设置LCD参数	 
	LCD_BL=1;//点亮背光	 
}
/*************************************************
函数名：LCD_SetWindows
功能：设置lcd显示窗口，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	if(lcddev.dir==1)//横屏
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

	LCD_WriteRAM_Prepare();	//开始写入GRAM				
}   

/*************************************************
函数名：LCD_SetCursor
功能：设置光标位置
入口参数：xy坐标
返回值：无
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos);

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos);

	LCD_WriteRAM_Prepare();	//开始写入GRAM	
} 

//设置LCD参数
//方便进行横竖屏模式切换
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x22;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=320;
	lcddev.height=240;
	lcddev.setxcmd=0x21;
	lcddev.setycmd=0x20;			
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0xA700);
	
		//参数1右下角为原点
  /*LCD_WriteReg(0x01,0x0000);
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0xA700);*/
	//参数2左上角为原点	
	LCD_WriteReg(0x01,0x0100); 
	LCD_WriteReg(0x03,0x0038);
	LCD_WriteReg(0x0060,0x2700);

#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width=240;
	lcddev.height=320;
	lcddev.setxcmd=0x20;
	lcddev.setycmd=0x21;	
	//参数1右下角为原点
	LCD_WriteReg(0x01,0x0000);
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x0060,0x2700);
	//参数2左上角为原点
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





