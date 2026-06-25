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

volatile unsigned char modo = 0;

//=============================================================================
// ADC
//=============================================================================

void ADC_Init(){
    ANSEL = 0x01;
    ANSELH = 0x00;     // PORTB digital

    ADCON0 = 0x81;
    ADCON1 = 0x80;
}

unsigned int ADC_Read(){
    __delay_ms(5);

    GO_nDONE = 1;

    while(GO_nDONE);

    return ((ADRESH << 8) + ADRESL);
}

//=============================================================================
// INTERRUPCIÓN EXTERNA RB0
//=============================================================================

void INT_Init(){

    TRISBbits.TRISB0 = 1;

    // Pull-up interno
    OPTION_REGbits.nRBPU = 0;

    // Interrupción por flanco descendente
    OPTION_REGbits.INTEDG = 0;

    INTF = 0;
    INTE = 1;
    GIE = 1;
}

//=============================================================================
// ISR
//=============================================================================

void __interrupt() ISR(void){

    if(INTF){

        modo++;

        if(modo > 2)
            modo = 0;

        INTF = 0;
    }
}

//=============================================================================
// MAIN
//=============================================================================

void main(void){

    ADC_Init();
    INT_Init();

    LCD lcd={&PORTC,2,3,4,5,6,7};
    LCD_Init(lcd);

    char buffer[16];

    while(1){

        unsigned int adc_result = ADC_Read();

        LCD_Clear();
        __delay_ms(2);

        //=================================================
        // VOLTAJE
        //=================================================
        if(modo == 0){

            LCD_Set_Cursor(0,0);
            LCD_putrs("Voltaje:");

            unsigned long volt =
                ((unsigned long)adc_result * 50000)/1023;

            unsigned int part_int = volt/10000;
            unsigned int part_dec = volt%10000;

            sprintf(buffer,"%u.%04u",part_int,part_dec);

            LCD_Set_Cursor(0,9);
            LCD_putrs(buffer);
        }

        //=================================================
        // PORCENTAJE
        //=================================================
        else if(modo == 1){

            LCD_Set_Cursor(0,0);
            LCD_putrs("Porcentaje:");

            unsigned int porcentaje =
                ((unsigned long)adc_result * 100UL)/1023UL;

            sprintf(buffer,"%u%%",porcentaje);

            LCD_Set_Cursor(1,0);
            LCD_putrs(buffer);
        }

        //=================================================
        // ADC
        //=================================================
        else if(modo == 2){

            LCD_Set_Cursor(0,0);
            LCD_putrs("ADC:");

            sprintf(buffer,"%u",adc_result);

            LCD_Set_Cursor(1,0);
            LCD_putrs(buffer);
        }

        __delay_ms(200);
    }
}
