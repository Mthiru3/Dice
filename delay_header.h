#include<LPC21XX.H>
void delay_microseconds(unsigned int);
void delay_milliseconds(unsigned int);
void delay_seconds(unsigned int);

void delay_milliseconds(unsigned int s)
{
// 	unsigned int i;
//	for(;s>0;s--)
//		for(i=0;i<= 12000;i++);
		T0PR=15000-1;
		T0TCR=0X01;
		while(T0TC<s);
		T0TCR=0X03;
		T0TCR=0X00;
}

void delay_microseconds(unsigned int s)
{
 //	unsigned int i;
//	for(;s>0;s--)
//		for(i=0;i<=12;i++);
		T0PR=15-1;
		T0TCR=0X01;
		while(T0TC<s);
		T0TCR=0X03;
		T0TCR=0X00;
}

void delay_seconds(unsigned int s)
{
 //	unsigned int i=0;
//	for(;s>0;s--)
//		for(i=0;i<=12000000;i++);
		T0PR=15000000-1;
		T0TCR=0X01;
		while(T0TC<s);
		T0TCR=0X03;
		T0TCR=0X00;
}
