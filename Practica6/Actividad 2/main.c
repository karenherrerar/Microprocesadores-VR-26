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

volatile unsigned char mensaje = 0;
unsigned char mensaje_anterior = 255;

// INTERRUPCIÓN EXTERNA
void LCD_CreateChar(unsigned char location, const unsigned char pattern[])
{
    unsigned char i;

    location &= 0x07;          // posiciones 0-7

    LCD_Cmd(0x40 + (location << 3));   // CGRAM

    for(i=0; i<8; i++)
    {
        LCD_putc(pattern[i]);
    }

    LCD_Cmd(0x80);             // Regresar a DDRAM
}

void __interrupt() ISR(void){

    if(INTF){

        __delay_ms(20);     // antirrebote

        if(INTF){

            mensaje++;

            if(mensaje > 2){
                mensaje = 0;
            }
        }

        INTF = 0;
    }
}

// MAIN

const unsigned char carita[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

const unsigned char corazon[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

void main(void){

    // Desactivar entradas analógicas
    ANSEL = 0;
    ANSELH = 0;

    // Pull-ups de PORTB
    OPTION_REGbits.nRBPU = 0;

    // Configuración de puertos
    TRISB = 0xFF;      // RB0 como entrada
    TRISC = 0x00;      // LCD

    PORTB = 0;
    PORTC = 0;

    // LCD

    LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};

    LCD_Init(lcd);
    LCD_Clear();
    LCD_CreateChar(0, carita);
    LCD_CreateChar(1, corazon);

    // INTERRUPCIÓN EXTERNA RB0
    GIE = 1;       // Interrupciones globales
    INTE = 1;      // Interrupción externa RB0
    INTF = 0;      // Limpia bandera
    INTEDG = 0;    // Flanco de bajada

    // CICLO PRINCIPAL
    while(1){

        if(mensaje != mensaje_anterior){

            LCD_Clear();

            switch(mensaje){

                case 0:

                    LCD_Set_Cursor(0,0);
                    LCD_putrs("Mensaje 1");

                    LCD_Set_Cursor(1,0);
                    LCD_putrs("Hello World");
                    break;

                case 1:

                    LCD_Set_Cursor(0,0);
                    LCD_putrs("Mensaje 2");

                    LCD_Set_Cursor(1,0);
                    LCD_putrs("PAPOI");
                    LCD_putc(1);
                    break;

                case 2:

                    LCD_Set_Cursor(0,0);
                    LCD_putrs("Mensaje 3");

                    LCD_Set_Cursor(1,0);
                    LCD_putrs("Buen Dia");
                    LCD_putc(0);
                    break;
            }

            mensaje_anterior = mensaje;
        }
    }
}


