#include <xc.h>

//================================================
// CONFIGURACION
//================================================

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

//================================================
// ADC
//================================================

void ADC_Init(void)
{
    ANSEL = 0x01;      // AN0 analógico
    ANSELH = 0x00;

    TRISA0 = 1;        // AN0 entrada

    ADCON0 = 0x01;     // ADC ON canal 0
    ADCON1 = 0x80;     // Justificado a la derecha
}

unsigned int ADC_Read(void)
{
    __delay_us(20);    // tiempo de adquisición

    GO_nDONE = 1;      // iniciar conversión

    while(GO_nDONE);   // esperar resultado

    return ((ADRESH << 8) + ADRESL);
}

//================================================
// PWM CCP1
//================================================

void PWM_Init(void)
{
    TRISC2 = 0;            // RC2 salida CCP1

    PR2 = 124;             // PWM ? 4 kHz

    CCP1CON = 0b00001100;  // CCP1 modo PWM

    T2CON = 0b00000101;    // Timer2 ON, prescaler 1:4
}

void PWM_SetDuty(unsigned int duty)
{
    if(duty > 500)
        duty = 500;

    CCPR1L = duty >> 2;

    CCP1CONbits.DC1B1 = (duty & 0x02) >> 1;
    CCP1CONbits.DC1B0 = duty & 0x01;
}

//================================================
// MAIN
//================================================

void main(void)
{
    unsigned int adc;
    unsigned int velocidad;

    ADC_Init();
    PWM_Init();

    // RB4 y RB5 entradas (botones)
    TRISB0 = 1;
    TRISB1 = 1;

    // RB0 y RB1 salidas (IN1 e IN2)
    TRISD0 = 0;
    TRISD1 = 0;

    // Pull-ups internos PORTB
    OPTION_REGbits.nRBPU = 0;

    PORTB = 0x00;

    while(1)
    {
        // Leer potenciómetro
        adc = ADC_Read();

        // Convertir 0-1023 a 0-500
        velocidad = (unsigned long)adc * 500 / 1023;

        // Actualizar PWM
        PWM_SetDuty(velocidad);

        // Giro horario
        if(RB0 == 0)
        {
            RD0 = 1;   // IN1
            RD1 = 0;   // IN2
        }

        // Giro antihorario
        else if(RB1 == 0)
        {
            RD0 = 0;   // IN1
            RD1 = 1;   // IN2
        }

        __delay_ms(10);
    }
}

