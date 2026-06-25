#include <xc.h>
#include <stdbool.h>
#include "lcd.h"
#include "keypad.h"
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

void main(void){
    LCD lcd = {&PORTC,2,3,4,5,6,7};

    char tecla;
    ANSEL = 0x00;
    ANSELH = 0x00;
    TRISC = 0x00;

    // Inicializar LCD
    LCD_Init(lcd);
    InitKeypad();
    while(1)
    {
        // Espera hasta que se presione una tecla
        tecla = switch_press_scan();

        // Limpia LCD

        // Muestra la tecla presionada
        LCD_putc(tecla);

        __delay_ms(200);
    }
}