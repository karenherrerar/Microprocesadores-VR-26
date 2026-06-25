#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACI N DE BITS DE CONFIGURACI N (FUSES)
//=============================================================================

// Selecci n de oscilador (usar XT si est s usando un cristal de 4 MHz)
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
unsigned char Estado1,Estado2,Estado3;

void main(void){
    ANSEL=0; 
    ANSELH=0;
    
    OPTION_REG=OPTION_REG & 0b01111111;
    
    TRISB=0xFF;
    TRISD=0x00;
    TRISC=0x00;
    PORTC=0;
    PORTD=0;
    PORTB=0;
    
    unsigned char cont=0;
    unsigned char unidades, decenas;

    const unsigned char display[10] = {
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
    while(1){
        
    // Botón sumar
    if(PORTBbits.RB0 == 0){

        __delay_ms(20);

        if(PORTBbits.RB0 == 0){

            cont++;

            if(cont > 99)
                cont = 0;

            while(PORTBbits.RB0 == 0);
        };
    };

    // Botón restar
    if(PORTBbits.RB1 == 0){

        __delay_ms(20);

        if(PORTBbits.RB1 == 0){

            if(cont == 0)
                cont = 99;
            else
                cont--;

            while(PORTBbits.RB1 == 0);
        };
    };

    // Botón reset
    if(PORTBbits.RB2 == 0){

        __delay_ms(20);

        if(PORTBbits.RB2 == 0){

            cont = 0;

            while(PORTBbits.RB2 == 0);
        };
    };
    
    decenas = cont / 10;
    unidades = cont % 10;
    PORTD = display[decenas];
    PORTC = display[unidades];
        
    };
};

