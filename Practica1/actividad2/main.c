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

#define _XTAL_FREQ 12000000      // Frecuencia del oscilador (para __delay_ms y __delay_us)

void main(void){
    TRISD=0b00000000;    //cada 0 es un bit diferente, si todos los bits son 0 significa que todos los pines son salidas
    PORTD=0b00000000;    //manda 0 a todos para que no haya ningun voltaje residual
    
    while(1){
        PORTD = 0b000000; 
        __delay_ms(500); // 0
        PORTD = 0b000001; 
        __delay_ms(500); // 1
        PORTD = 0b000010; 
        __delay_ms(500); // 2
        PORTD = 0b000011; 
        __delay_ms(500); // 3
        PORTD = 0b000100; 
        __delay_ms(500); // 4
        PORTD = 0b000101; 
        __delay_ms(500); // 5
        PORTD = 0b000110; 
        __delay_ms(500); // 6
        PORTD = 0b000111; 
        __delay_ms(500); // 7
        PORTD = 0b001000; 
        __delay_ms(500); // 8
        PORTD = 0b001001; 
        __delay_ms(500); // 9
        PORTD = 0b001010; 
        __delay_ms(500); // 10
        PORTD = 0b001011; 
        __delay_ms(500); // 11
        PORTD = 0b001100; 
        __delay_ms(500); // 12
        PORTD = 0b001101; 
        __delay_ms(500); // 13
        PORTD = 0b001110; 
        __delay_ms(500); // 14
        PORTD = 0b001111; 
        __delay_ms(500); // 15
        PORTD = 0b010000; 
        __delay_ms(500); // 16
        PORTD = 0b010001; 
        __delay_ms(500); // 17
        PORTD = 0b010010; 
        __delay_ms(500); // 18
        PORTD = 0b010011; 
        __delay_ms(500); // 19
        PORTD = 0b010100; 
        __delay_ms(500); // 20
        PORTD = 0b010101; 
        __delay_ms(500); // 21
        PORTD = 0b010110; 
        __delay_ms(500); // 22
        PORTD = 0b010111; 
        __delay_ms(500); // 23
        PORTD = 0b011000; 
        __delay_ms(500); // 24
        PORTD = 0b011001; 
        __delay_ms(500); // 25
        PORTD = 0b011010; 
        __delay_ms(500); // 26
        PORTD = 0b011011; 
        __delay_ms(500); // 27
        PORTD = 0b011100; 
        __delay_ms(500); // 28
        PORTD = 0b011101; 
        __delay_ms(500); // 29
        PORTD = 0b011110; 
        __delay_ms(500); // 30
        PORTD = 0b011111; 
        __delay_ms(500); // 31
        PORTD = 0b100000; 
        __delay_ms(500); // 32
        PORTD = 0b100001; 
        __delay_ms(500); // 33
        PORTD = 0b100010; 
        __delay_ms(500); // 34
        PORTD = 0b100011; 
        __delay_ms(500); // 35
        PORTD = 0b100100; 
        __delay_ms(500); // 36
        PORTD = 0b100101; 
        __delay_ms(500); // 37
        PORTD = 0b100110; 
        __delay_ms(500); // 38
        PORTD = 0b100111; 
        __delay_ms(500); // 39
        PORTD = 0b101000; 
        __delay_ms(500); // 40
        PORTD = 0b101001; 
        __delay_ms(500); // 41
        PORTD = 0b101010; 
        __delay_ms(500); // 42
        PORTD = 0b101011; 
        __delay_ms(500); // 43
        PORTD = 0b101100; 
        __delay_ms(500); // 44
        PORTD = 0b101101; 
        __delay_ms(500); // 45
        PORTD = 0b101110; 
        __delay_ms(500); // 46
        PORTD = 0b101111; 
        __delay_ms(500); // 47
        PORTD = 0b110000; 
        __delay_ms(500); // 48
        PORTD = 0b110001; 
        __delay_ms(500); // 49
        PORTD = 0b110010; 
        __delay_ms(500); // 50
        PORTD = 0b110011; 
        __delay_ms(500); // 51
        PORTD = 0b110100; 
        __delay_ms(500); // 52
        PORTD = 0b110101; 
        __delay_ms(500); // 53
        PORTD = 0b110110; 
        __delay_ms(500); // 54
        PORTD = 0b110111; 
        __delay_ms(500); // 55
        PORTD = 0b111000; 
        __delay_ms(500); // 56
        PORTD = 0b111001; 
        __delay_ms(500); // 57
        PORTD = 0b111010; 
        __delay_ms(500); // 58
        PORTD = 0b111011; 
        __delay_ms(500); // 59
        PORTD = 0b111100;
        __delay_ms(500); // 60
        PORTD = 0b111101;
        __delay_ms(500); // 61
        PORTD = 0b111110; 
        __delay_ms(500); // 62
        PORTD = 0b111111; 
        __delay_ms(500); // 63 :p     
    }
}

