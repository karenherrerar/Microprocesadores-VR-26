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

#define _XTAL_FREQ 12000000

//=============================================================================
// ADC
//=============================================================================

void ADC_Init(){

    ANSEL = 0x03;      // AN0 y AN1 analógicos
    ANSELH = 0x00;     // Resto digitales

    ADCON0 = 0x01;     // ADC encendido
    ADCON1 = 0x80;     // Justificado a la derecha
}

unsigned int ADC_Read(unsigned char canal){

    ADCON0 &= 0xC3;            // Limpia CHS3:CHS0
    ADCON0 |= (canal << 2);    // Selecciona canal

    __delay_us(50);

    GO_nDONE = 1;
    while(GO_nDONE);

    return ((ADRESH << 8) + ADRESL);
}

//=============================================================================
// PWM
//=============================================================================

void PWM_Init(){

    TRISC1 = 0;      // CCP2
    TRISC2 = 0;      // CCP1

    PR2 = 124;

    CCP1CON = 0b00001100;
    CCP2CON = 0b00001100;

    T2CON = 0b00000101;   // Timer2 ON, prescaler 1:4
}

void PWM1_Set(unsigned int valor){

    CCPR1L = valor >> 2;

    CCP1CONbits.DC1B1 = (valor & 0x02) >> 1;
    CCP1CONbits.DC1B0 = valor & 0x01;
}

void PWM2_Set(unsigned int valor){

    CCPR2L = valor >> 2;

    CCP2CONbits.DC2B1 = (valor & 0x02) >> 1;
    CCP2CONbits.DC2B0 = valor & 0x01;
}

//=============================================================================
// NIVELES DE BRILLO
//=============================================================================

unsigned int Nivel_Brillo(unsigned int adc){

    if(adc < 205)
        return 0;

    else if(adc < 410)
        return 125;

    else if(adc < 615)
        return 250;

    else if(adc < 819)
        return 375;

    else
        return 500;
}

//=============================================================================
// MAIN
//=============================================================================

void main(){

    unsigned int adc1;
    unsigned int adc2;

    unsigned int brillo1;
    unsigned int brillo2;

    TRISA = 0xFF;

    ADC_Init();
    PWM_Init();

    while(1){

        adc1 = ADC_Read(0);
        __delay_ms(5);

        adc2 = ADC_Read(1);
        __delay_ms(5);

        brillo1 = Nivel_Brillo(adc1);
        brillo2 = Nivel_Brillo(adc2);

        PWM1_Set(brillo1);
        PWM2_Set(brillo2);
    }
}