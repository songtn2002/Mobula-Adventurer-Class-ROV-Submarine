void LCD_SetWindows(u16 x1, u16 y1,u16 x2,u16 y2)
{		
	LCD_WR_REG(0x50);	
	LCD_WR_DATA(x1);
	LCD_WR_REG(0x51);		
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x52);	
	LCD_WR_DATA(y1);
	LCD_WR_REG(0x53);		
	LCD_WR_DATA(y2);
	
	LCD_WR_REG(0x20);	
	LCD_WR_DATA(x1);	
	LCD_WR_REG(0x21);	
	LCD_WR_DATA(y2);
	LCD_Write_COM(0x22); 	
}   
	

