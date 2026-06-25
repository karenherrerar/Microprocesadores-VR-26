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

// VARIABLES
volatile unsigned int tiempo=0;
volatile unsigned int contador=0;
char exec[6];
//FUNCIONES

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
    ADC_Init();
    
    LCD lcd = {&PORTC,2,3,4,5,6,7};
    LCD_Init(lcd);
    char buffer[10];
    
    LCD_Clear();

    LCD_Set_Cursor(1,10);
    LCD_putrs("00:00");
    
    
    
    while(1){
        LCD_Set_Cursor(0,0);
        LCD_putrs("Voltaje: ");
        LCD_Set_Cursor(0,9);
        unsigned int adc_result=ADC_Read();
        unsigned int volt=(adc_result*50000)/1023;  //se pasan los decimales a enteros
        unsigned int part_int=volt/10000;
        unsigned int part_dec=volt%10000;
        sprintf(buffer,"%u.%u",part_int, part_dec);
        LCD_putrs(buffer);
        
        LCD_Set_Cursor(1,10);
        sprintf(exec,"%02u:%02u",tiempo/60,tiempo%60);
        LCD_putrs(exec);
    }
}