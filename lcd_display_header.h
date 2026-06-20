#include<lpc21xx.h>
#include"delay_header.h"

#define LCD 0xff
#define RS 1<<8
#define E 1<<9

void LCD_INIT(void);
void LCD_COMMAND(char);
void LCD_DATA(char);
void LCD_STR(char *);
void LCD_INTEGER(int);
void LCD_FLOAT(float);
void LCD_CGRAM(unsigned int);
unsigned char cgram[];


void LCD_INIT()
{
	IODIR0 |=LCD|RS|E;
	LCD_COMMAND(0X01);//clear the display
	LCD_COMMAND(0X02);//return cursor to home position
	LCD_COMMAND(0X0c);//display on,cursor off
	LCD_COMMAND(0X38);//8 bit interface mode with both lines
}

void LCD_COMMAND(char cmd)
{
	IOCLR0=LCD;//clear thye data pins
	IOSET0=cmd;//assign commend byte to data pins
	IOCLR0=RS;//set RS=0 for command Register mode,
	          //In RS:RS=1 for data mode
			  //and RS=0 for command mode 
	IOSET0=E;//E=1 for enable latch
	delay_milliseconds(2);//delay for data to assign
	IOCLR0=E;// E=0  to relatch for next byte
}

void LCD_DATA(char d)
{
	IOCLR0=LCD;//Clear the data pins
	IOSET0=d;// assign value to data pin
	IOSET0=RS;//set RS=1 for data register mode
	IOSET0=E;// set E=1 for latch data byte
	delay_milliseconds(2);
	IOCLR0=E;// set E=1 for relatch to next byte
}

void LCD_STR(char *s)
{
	unsigned char i;
	for(i=0;s[i];i++)// until null(\0) or zero comes
	LCD_DATA(s[i]);
}
/* 
void LCD_STR(char *s)
{
	while(*s)  //check for null character '\0'
	LCD_DATA(*s++);
}
*/

void LCD_INTEGER(int n)
{
	unsigned int arr[10],i=0;
	if(n==0)
	  LCD_DATA('0');
	else
	{
	  if(n<0)
	  {
        LCD_DATA('-');
	    n=-n;
	  }
	   while(n!=0)
	   {
	    arr[i++]=(n%10)+48;
	    n=n/10;
	   }
	   while(i>0)
	   LCD_DATA(arr[--i]);
     }
}

void LCD_FLOAT(float f)
{
	unsigned int temp;
	temp=f;
	LCD_INTEGER(temp);
	LCD_DATA('.');
	f=f-temp;
	temp=f*10000;
	LCD_INTEGER(temp);
}

void LCD_CGRAM(unsigned int nbytes)
{
	unsigned int i;
	LCD_COMMAND(0X40);//Starting address of the CGRAM
	for(i=0;i<nbytes;i++)// nbytes--> no of symbols (per symbol 8 byte)
	LCD_DATA(cgram[i]);//assign or store byte value in CGRAM Address
}

