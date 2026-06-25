#include <stdlib.h>
#include <stdio.h>
#include <xc.h>
#include <stdbool.h>
#include "lcd.h"
//=============================================================================
// CONFIGURACIÓN
//=============================================================================

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

void main (void){
   LCD lcd= {&PORTC, 2, 3, 4, 5, 6, 7};//PORT,RS,EN,D4,D5,D6,D7
   LCD_Init(lcd);
   while(1){
       LCD_Clear();
       LCD_Set_Cursor(0,0); 
       LCD_putrs("Hello World!");
       
       LCD_Set_Cursor(1,0);
       for (char c='A';c<'Q';c++){
         LCD_putc(c); 
         __delay_ms(300);
       }
       __delay_ms(1000);    
   };
};
