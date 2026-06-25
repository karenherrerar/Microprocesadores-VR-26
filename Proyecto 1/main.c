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

volatile unsigned char carita = 0; // 0=feliz 1=triste

unsigned char posX = 0;
unsigned char posY = 0;

unsigned int velocidadX;

//=============================================================================
// ADC
//=============================================================================

void ADC_Init(){

    ANSEL = 0x03;
    ANSELH = 0x00;

    ADCON0 = 0x01;
    ADCON1 = 0x80;
}

void DelayVariable(unsigned int tiempo){

    unsigned int i;

    for(i = 0; i < tiempo; i++){
        __delay_ms(1);
    }
}
unsigned int ADC_Read(unsigned char channel){

    __delay_us(5);

    ADCON0 &= 0x83;
    ADCON0 |= (channel << 2);

    __delay_ms(2);

    GO_nDONE = 1;

    while(GO_nDONE);

    return ((ADRESH << 8) + ADRESL);
}

//=============================================================================
// LCD
//=============================================================================

void LCD_CreateChar(unsigned char location, const unsigned char pattern[]){

    unsigned char i;

    location &= 0x07;

    LCD_Cmd(0x40 + (location << 3));

    for(i = 0; i < 8; i++){
        LCD_putc(pattern[i]);
    }

    LCD_Cmd(0x80);
}

//=============================================================================
// INTERRUPCIÓN EXTERNA
//=============================================================================

void INT_Init(){

    TRISBbits.TRISB0 = 1;

    OPTION_REGbits.nRBPU = 0;
    OPTION_REGbits.INTEDG = 0;

    INTF = 0;
    INTE = 1;
    GIE = 1;
}

void __interrupt() ISR(void){

    if(INTF){

        carita++;

        if(carita > 1){
            carita = 0;
        }

        INTF = 0;
    }
}

//=============================================================================
// CARACTERES PERSONALIZADOS
//=============================================================================

unsigned char feliz[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

unsigned char triste[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b01110,
    0b10001,
    0b00000,
    0b00000
};

void main(void){

    unsigned int ejeX = 0;
    unsigned int ejeY = 0;

    LCD lcd = {&PORTC,2,3,4,5,6,7};

    LCD_Init(lcd);
    LCD_Clear();

    ADC_Init();
    INT_Init();

    LCD_CreateChar(0, feliz);
    LCD_CreateChar(1, triste);

    while(1){

        ejeX = ADC_Read(0);   // AN0
        ejeY = ADC_Read(1);   // AN1

        if(ejeX > 550){

            velocidadX = 300 - ((ejeX - 550) / 2);

            if(velocidadX < 50){
                velocidadX = 50;
            }

            if(posX == 15){
                posX = 0;
            }
            else{
                posX++;
            }

            DelayVariable(velocidadX);
        }

        else if(ejeX < 470){

            velocidadX = 300 - ((470 - ejeX) / 2);

            if(velocidadX < 50){
                velocidadX = 50;
            }

            if(posX == 0){
                posX = 15;
            }
            else{
                posX--;
            }

            DelayVariable(velocidadX);
        }


        if(ejeY > 700){

            if(posY == 0){
                posY = 1;
            }
            else{   
                posY = 0;
            }

            __delay_ms(150);
        }

        else if(ejeY < 300){

            if(posY == 1){
                posY = 0;
            }
            else{
                posY = 1;
            }

            __delay_ms(150);
        }

        LCD_Clear();

        LCD_Set_Cursor(posY, posX);

        if(carita == 0){
            LCD_putc(0);
        }
        else{
            LCD_putc(1);
        }
    }
}
