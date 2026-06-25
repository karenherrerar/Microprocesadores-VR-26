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
    ANSEL=0x01;
    ADCON0=0x81;
    ADCON1=0x80; //justificacion a la derecha
};

unsigned int ADC_Read(){
    __delay_ms(5);//asegura que el capacitor se cargue
    GO_nDONE=1;
    while(GO_nDONE); //espera hasta que termina
    return(ADRESH<<8)+ADRESL;   //Suma los dos regitros recorriendo 8bits Adresh y sumando adresl
}

void main (void){
    ADC_Init();
    LCD lcd={&PORTC,2,3,4,5,6,7};
    LCD_Init(lcd);
    
    char buffer[10];
    while(1){
        LCD_Clear();
        LCD_Set_Cursor(0,0);
        LCD_putrs("Voltaje: ");
        LCD_Set_Cursor(0,9);
        
        unsigned int adc_result=ADC_Read();
        
        //no optimizado
//        float volt=adc_result*5.0/1023;
//        sprintf(buffer, "%.1f",volt); //convierte el valor de adc a cadena de texto
//        LCD_putrs(buffer);
        
        unsigned int volt=(adc_result*50000)/1023;  //se pasan los decimales a enteros
        unsigned int part_int=volt/10000;
        unsigned int part_dec=volt%10000;
        sprintf(buffer,"%u.%u",part_int, part_dec);
        LCD_putrs(buffer);
        __delay_ms(200);
    };
}


