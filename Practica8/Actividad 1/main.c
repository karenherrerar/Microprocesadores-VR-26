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

void ADC_Init(){
    ANSEL=0x03;
    ANSELH=0;
    ADCON0=0x01;
    ADCON1=0x80;
};

unsigned int ADC_Read(unsigned char channel){
    __delay_us(5);
    ADCON0 &= 0x83;//ADCON0 & 0x83
    ADCON0 |= channel << 2; //Sustituir los bits del canal por el seleccionado
    __delay_ms(2);
    //empieza lectura analogica
    GO_nDONE=1;
    while(GO_nDONE);
    return((ADRESH<<8)+ADRESL);
}

void main (void){
   char buffer1[10];
   char buffer2[10];
   
   ADC_Init();
   LCD lcd={&PORTC,2,3,4,5,6,7};
   LCD_Init(lcd);
   
   while(1){
       LCD_Clear();
       LCD_Set_Cursor(0,0);
       LCD_putrs("Voltaje 1:");
       LCD_Set_Cursor(0,11);
       unsigned int adc_result1= ADC_Read(0);
       unsigned int volt1=(adc_result1*50000)/1023;
       unsigned int part_ent1= volt1/10000;
       unsigned int part_dec1=volt1%10000;
       sprintf(buffer1,"%u.%u",part_ent1,part_dec1);
       LCD_putrs(buffer1);
       __delay_ms(100);
       
       LCD_Set_Cursor(1,0);
       LCD_putrs("Voltaje 2:");
       LCD_Set_Cursor(1,11);
       unsigned int adc_result2= ADC_Read(1);
       unsigned int volt2=(adc_result2*50000)/1023;
       unsigned int part_ent2= volt2/10000;
       unsigned int part_dec2=volt2%10000;
       sprintf(buffer2,"%u.%u",part_ent2,part_dec2);
       LCD_putrs(buffer2);
       __delay_ms(100);
   }
   
};
