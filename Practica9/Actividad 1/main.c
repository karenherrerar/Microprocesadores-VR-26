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

//Definicion variables
volatile unsigned int tiempo=0;
volatile unsigned int contador=0;
char exec[6];

//timer0 8bits   0-255 --> desb./int.interna  ciclo=Fosc/4  
void Timer0_Init(){
    OPTION_REG=0x07; //establece el prescaler
    TMR0=178;
    T0IE=1;
    GIE=1; //int.globales
}
void __interrupt () ISR(void){
    if (T0IF){
        contador++;
        if(contador>100){
            tiempo++;
            contador=0;
        }
        TMR0=178;
        T0IF=0; 
    }
}

void main (void){
    Timer0_Init();
    
    LCD lcd = {&PORTC,2,3,4,5,6,7};
    LCD_Init(lcd);
    
    LCD_Clear();
    LCD_Set_Cursor(0,0);
    LCD_putrs("Tiempo: ");
    LCD_Set_Cursor(1,0);
    LCD_putrs("00:00");
    
    while(1){
        LCD_Set_Cursor(1,0);
        sprintf(exec,"%02u:%02u",tiempo/60,tiempo%60);
        LCD_putrs(exec);
    }
}
