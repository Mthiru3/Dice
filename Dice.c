#include"lcd_display_header.h"
unsigned char cgram[]	={0x1f,0x11,0x11,0x15,0x11,0x11,0x11,0x1f,
                          0x1f,0x11,0x19,0x11,0x13,0x11,0x11,0x1f,
						  0x1f,0x11,0x19,0x15,0x13,0x11,0x11,0x1f,
                          0x1f,0x11,0x1B,0x11,0x1B,0x11,0x11,0x1f,
						  0x1f,0x11,0x1B,0x15,0x1B,0x11,0x11,0x1f,
						  0x1f,0x1B,0X11,0x1B,0X11,0x1B,0x11,0x1f};

int count=0;

void dice_eint(void)__irq
{
	EXTINT=0X01;
	count=T0TC%6;
	VICVectAddr=0;	
}

int main()
{
	int i;
	PINSEL1=0X1;
	LCD_INIT();
	LCD_CGRAM(48);
	T0TCR=0X02;
	T0TCR=0X01;
	VICIntSelect=0;
	VICVectCntl0=(1<<5)|14;
	VICVectAddr0=(unsigned long)dice_eint;
	EXTMODE=0X01;
	EXTPOLAR=0X00;
	VICIntEnable=1<<14;
	while(1)
	{
		LCD_COMMAND(0X80);
//		LCD_STR("ROLL THE DICE... ");//for 20*4 display
		LCD_STR("ROLL THE DICE.."); //for 16*2 display
		for(i=0;i<6;i++)
		{
//			LCD_COMMAND(0X91);//for 20*4 display
			LCD_COMMAND(0X8f);//for 16*2 display
			LCD_DATA(i);
			delay_milliseconds(100);
		}
		if(count!=0)
		{
			LCD_COMMAND(0X01);
// 			LCD_STR("RELEASE TO PICK !!!");//for 20*4 display
 			LCD_STR("RELEASE TO PICK!");//for 16*2 display
			while(((IOPIN0>>16)&1)==0);
			delay_milliseconds(5);
			LCD_COMMAND(0X01);
			LCD_COMMAND(0xc0);
			LCD_STR("YOUR PICK ");
			switch(count)
			{
				case 0 : LCD_DATA(0);break;
				case 1 : LCD_DATA(1);break;
				case 2 : LCD_DATA(2);break;
				case 3 : LCD_DATA(3);break;
				case 4 : LCD_DATA(4);break;
				case 5 : LCD_DATA(5);break;
			}
			count=0;
			delay_milliseconds(2000);
			LCD_COMMAND(0X01);
		}	  
	}
}
