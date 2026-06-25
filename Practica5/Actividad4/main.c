#include <xc.h>

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

volatile unsigned char direccion = 1; // 1=sube, 0=baja

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

//=============================================================================
// PROGRAMA PRINCIPAL
//=============================================================================

void main(void){

    ANSEL = 0;
    ANSELH = 0;

    OPTION_REG = OPTION_REG & 0b01111111; // Pull-ups PORTB

    TRISB = 0xFF; // RB0 entrada INT
    TRISC = 0x00; // Selección displays
    TRISD = 0x00; // Segmentos

    PORTB = 0;
    PORTC = 0xFF;
    PORTD = 0;

    int num = 0;
    int miles, centenas, decenas, unidades;
    char i;

    //=============================
    // INTERRUPCIÓN EXTERNA
    //=============================

    GIE = 1;       // Interrupciones globales
    INTE = 1;      // INT externa RB0
    INTF = 0;      // Limpia bandera
    INTEDG = 0;    // Flanco de bajada

    while(1){

        miles    = (num / 1000) % 10;
        centenas = (num / 100) % 10;
        decenas  = (num / 10) % 10;
        unidades = num % 10;

        // MULTIPLEXACIÓN

        for(i = 0; i < 10; i++){

            // Unidades
            PORTC = 0b11111101;
            PORTD = patron[unidades];
            __delay_ms(1);

            // Decenas
            PORTC = 0b11111110;
            PORTD = patron[decenas];
            __delay_ms(1);

            // Centenas
            PORTC = 0b11111011;
            PORTD = patron[centenas];
            __delay_ms(1);

            // Miles
            PORTC = 0b11110111;
            PORTD = patron[miles];
            __delay_ms(1);
        }

        if(direccion){

            num++;

            if(num > 9999){
                num = 0;
            }

        }else{

            if(num == 0){
                num = 9999;
            }else{
                num--;
            }
        }
    }
}

void __interrupt() ISR(void){

    if(INTF){
        direccion = !direccion; // Cambia sentido
        INTF = 0;
    }
}
