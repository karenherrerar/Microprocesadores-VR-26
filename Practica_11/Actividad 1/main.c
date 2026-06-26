#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 12000000

void ADC_Init(){
    ANSEL = 0x01;
    ANSELH = 0x00;
    TRISA0 = 1;
    ADCON0 = 0x01;
    ADCON1 = 0x80;
}

unsigned int ADC_Read(unsigned char canal){
    ADCON0 &= 0xC3;
    ADCON0 |= (canal << 2);

    __delay_us(50);

    GO_nDONE = 1;
    while(GO_nDONE);

    return ((ADRESH << 8) + ADRESL);
}

void PWM_Init(){
    TRISC2 = 0;

    PR2 = 124;

    CCP1CON = 0b00001100;

    T2CON = 0b00000101;
}

void PWM1_Set(unsigned int valor){
    CCPR1L = valor >> 2;

    CCP1CONbits.DC1B1 = (valor & 0x02) >> 1;
    CCP1CONbits.DC1B0 = valor & 0x01;
}

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
        return 499;
}

void main(){
    unsigned int adc;
    unsigned int brillo;

    ADC_Init();
    PWM_Init();

    while(1){
        adc = ADC_Read(0);
        __delay_ms(5);

        brillo = Nivel_Brillo(adc);

        PWM1_Set(brillo);
    }
}
