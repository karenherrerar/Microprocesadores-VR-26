//practica 5 parte 1
#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACI N DE BITS DE CONFIGURACI N (FUSES)
//=============================================================================

// Seleccion de oscilador (usar XT si est s usando un cristal de 4 MHz)
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection (disabled)
#pragma config WRT = OFF        // Flash Program Memory Write Enable (disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (disabled)

//=============================================================================
// DEFINICIONES
//=============================================================================

#define _XTAL_FREQ 8000000      // Frecuencia del oscilador (para __delay_ms y __delay_us)

unsigned char patron[10] = {
        0x3F, //0
        0x06, //1
        0x5B, //2
        0x4F, //3
        0x66, //4
        0x6D, //5
        0x7D, //6
        0x07, //7
        0x7F, //8
        0x6F  //9
    };
void main (void){
    TRISD=0;
    TRISC=0;
    int num=0;
    while(1){
      int miles    = (num / 1000) % 10;
      int centenas = (num / 100) % 10;
      int decenas=(num/10)%10; //extraemos decenas
      int unidades= num%10; //extraemos unidades
      // inicia multiplexacion
      for (int i=0; i<10; i++){
          //mostramos unidades
          PORTC=0b11111101;//se manda un cero a las unidades
          PORTD=patron[unidades];
          __delay_ms(1);
          
          //mostramos decenas
          PORTC=0b11111110;//se manda un cero a las decenas
          PORTD=patron[decenas];
          __delay_ms(1);
          
          //mostramos centenas
          PORTC=0b11111011;//se manda un cero a las centenas
          PORTD=patron[centenas];
          __delay_ms(1);
          
          //mostramos miles
          PORTC=0b11110111;//se manda un cero a las centenas
          PORTD=patron[miles];
          __delay_ms(1);
          
      };
      num++;
      
      if (num==10000){
          num=0;
      };
      
    };
};
