#include "lcd_display_header.h"
// 6 Custom characters for the dice faces (1-6)
unsigned char cgram[] = {
    0x1f,0x11,0x11,0x15,0x11,0x11,0x11,0x1f,
    0x1f,0x11,0x19,0x11,0x13,0x11,0x11,0x1f,
    0x1f,0x11,0x19,0x15,0x13,0x11,0x11,0x1f,
    0x1f,0x11,0x1B,0x11,0x1B,0x11,0x11,0x1f,
    0x1f,0x11,0x1B,0x15,0x1B,0x11,0x11,0x1f,
    0x1f,0x1B,0X11,0x1B,0X11,0x1B,0x11,0x1f
};
int count = 0;
// Interrupt Service Routine for EINT0
void dice_eint(void) __irq
{
    EXTINT = 0X01;          // Clear interrupt flag
    count = (T0TC % 6) + 1; // Shifting range to 1-6 so case 0/Face 1 is reachable
    VICVectAddr = 0;        // Acknowledge VIC
}

int main()
{
    int i;
    PINSEL1 = 0X1;          // Enable EINT0 on P0.16
    LCD_INIT();
    LCD_CGRAM(48);          // Load 48 bytes of custom graphics
    T0TCR = 0X02;           // Reset Timer 0
    T0TCR = 0X01;           // Start Timer 0
    
    // VIC Configuration
    VICIntSelect = 0;
    VICVectCntl0 = (1 << 5) | 14;
    VICVectAddr0 = (unsigned long)dice_eint;
    EXTMODE = 0X01;         // Edge-triggered
    EXTPOLAR = 0X00;        // Falling edge
    VICIntEnable = 1 << 14; // Enable EINT0 interrupt
    
    while(1)
    {
        LCD_COMMAND(0X80);                      // Line 1, Position 0
        LCD_STR("ROLL THE DICE... ");            // Optimized for 20x4 display
        for(i = 0; i < 6; i++)
        {
            LCD_COMMAND(0X91);                  // Line 2, Position 17 on 20x4 LCD
            LCD_DATA(i);                        // Cycle through custom dice animations
            delay_milliseconds(100);
        }
        if(count != 0)
        {
            LCD_COMMAND(0X01);                  // Clear Display
            LCD_STR("RELEASE TO PICK !!!");      // Optimized for 20x4 display
            while(((IOPIN0 >> 16) & 1) == 0);   // Wait until button P0.16 is released
            delay_milliseconds(5);              // Debounce delay
            LCD_COMMAND(0X01);                  // Clear Display
            LCD_COMMAND(0xc0);                  // Line 2, Position 0
            LCD_STR("YOUR PICK ");
            // Map the 1-6 count back to 0-5 CGRAM addresses
            switch(count)
            {
                case 1 : LCD_DATA(0); break;
                case 2 : LCD_DATA(1); break;
                case 3 : LCD_DATA(2); break;
                case 4 : LCD_DATA(3); break;
                case 5 : LCD_DATA(4); break;
                case 6 : LCD_DATA(5); break;
            }
            count = 0;                          // Reset flag for next roll
            delay_milliseconds(2000);           // Display result for 2 seconds
            LCD_COMMAND(0X01);
        }      
    }
}
