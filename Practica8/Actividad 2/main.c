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

//=============================================================================
// VARIABLES GLOBALES
//=============================================================================

volatile unsigned char modo = 0;     // 0=Voltaje, 1=Porcentaje, 2=ADC
unsigned char canal = 0;             // 0=AN0, 1=AN1

void ADC_Init(){
    ANSEL = 0x03;
    ANSELH = 0x00;
    ADCON0 = 0x01;
    ADCON1 = 0x80;
}

unsigned int ADC_Read(unsigned char channel){

    __delay_us(5);

    ADCON0 &= 0x83;
    ADCON0 |= channel << 2;

    __delay_ms(2);

    GO_nDONE = 1;

    while(GO_nDONE);

    return((ADRESH<<8)+ADRESL);
}

// INTERRUPCIÓN RB0
void INT_Init(){
    TRISBbits.TRISB0 = 1; // Botón modo
    TRISBbits.TRISB1 = 1; // Botón canal
    OPTION_REGbits.nRBPU = 0; // Pull-ups internos
    OPTION_REGbits.INTEDG = 0;
    INTF = 0;
    INTE = 1;
    GIE = 1;
}

// ISR
void __interrupt() ISR(void){
    if(INTF){
        modo++;
        if(modo > 2){
            modo = 0;
        }
        INTF = 0;
    }
}

// MAIN
void main (void){

   char buffer[16];

   ADC_Init();
   INT_Init();

   LCD lcd={&PORTC,2,3,4,5,6,7};
   LCD_Init(lcd);

   while(1){
       // BOTÓN RB1 CAMBIA ENTRE AN0 Y AN1
       if(PORTBbits.RB1 == 0){
           __delay_ms(20);
           if(PORTBbits.RB1 == 0){
               canal++;
               if(canal > 1){
                   canal = 0;
               }
               while(PORTBbits.RB1 == 0);
           }
       }

       LCD_Clear();
       unsigned int adc_result = ADC_Read(canal);
       
       // MODO VOLTAJE
       if(modo == 0){
           LCD_Set_Cursor(0,0);
           if(canal == 0)
               LCD_putrs("Voltaje 1:");
           else
               LCD_putrs("Voltaje 2:");
           LCD_Set_Cursor(1,0);
           unsigned int volt=(adc_result*50000)/1023;
           unsigned int part_ent=volt/10000;
           unsigned int part_dec=volt%10000;
           sprintf(buffer,"%u.%04u",part_ent,part_dec);
           LCD_putrs(buffer);
       }
       
       // MODO PORCENTAJE
       else if(modo == 1){
           LCD_Set_Cursor(0,0);
           if(canal == 0)
               LCD_putrs("Porcentaje V1");
           else
               LCD_putrs("Porcentaje V2");
           unsigned int porcentaje=
            ((unsigned long)adc_result*100)/1023;
           sprintf(buffer,"%u%%",porcentaje);
           LCD_Set_Cursor(1,0);
           LCD_putrs(buffer);
       }

       // MODO ADC
       else if(modo == 2){
           LCD_Set_Cursor(0,0);
           if(canal == 0)
               LCD_putrs("ADC V1:");
           else
               LCD_putrs("ADC V2:");
           sprintf(buffer,"%u",adc_result);
           LCD_Set_Cursor(1,0);
           LCD_putrs(buffer);
       }

       __delay_ms(200);
   }
}

