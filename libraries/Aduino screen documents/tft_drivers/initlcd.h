void LCD_GPIO_Init(void)
{
	......
//Init the GPIOs for LCD	
}
void LCD_RESET(void)
{
	LCD_RST_CLR;
	delay(100);	
	LCD_RST_SET;
	delay(50);
}
void LCD_Init(void)
{
	//LCD_GPIO_Init();//Init the GPIOs for LCD
	//LCD_RESET();//You should ResetLCD before Init.

  	//≥ı ºªØ
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

}

