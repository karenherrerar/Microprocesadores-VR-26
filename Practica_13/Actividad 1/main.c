#include <xc.h>

// CONFIGURACIÓN
#pragma config FOSC = HS     
#pragma config WDTE = OFF     
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

//======================================================
// INICIALIZAR ADC EN RA0 / AN0
//======================================================
void ADC_Init()
{
    ANSEL = 0x01;      // AN0 analógico, los demás digitales
    ANSELH = 0x00;     // AN8-AN13 digitales

    TRISA0 = 1;        // RA0 como entrada

    ADCON0 = 0x01;     // ADC encendido, canal AN0
    ADCON1 = 0x80;     // Justificado a la derecha, Vref = VDD y VSS
}

//======================================================
// LEER ADC
//======================================================
unsigned int ADC_Read()
{
    __delay_us(20);    // Tiempo de adquisición

    GO_nDONE = 1;      // Iniciar conversión

    while(GO_nDONE);   // Esperar a que termine

    return ((ADRESH << 8) + ADRESL);
}

//======================================================
// INICIALIZAR PWM EN RC2 / CCP1
//======================================================
void PWM_Init()
{
    TRISC2 = 0;            // RC2 como salida PWM

    PR2 = 124;             // Frecuencia PWM aprox. 4 kHz con Fosc = 8 MHz

    CCPR1L = 0;            // Duty inicial en 0
    CCP1CON = 0b00001100;  // CCP1 en modo PWM

    T2CON = 0b00000101;    // Timer2 ON, prescaler 1:4
}

//======================================================
// COLOCAR DUTY CYCLE
// duty debe ir de 0 a 499 con PR2 = 124
//======================================================
void PWM_SetDuty(unsigned int duty)
{
    if(duty > 499)
    {
        duty = 499;
    }

    CCPR1L = duty >> 2;              // 8 bits más significativos
    CCP1CONbits.DC1B0 = duty & 0x01; // bit menos significativo
    CCP1CONbits.DC1B1 = (duty & 0x02) >> 1;
}

//======================================================
// INICIALIZAR PINES DEL MOTOR
// RD0 -> IN1
// RD1 -> IN2
//======================================================
void Motor_Init()
{
    TRISD0 = 0;   // RD0 como salida
    TRISD1 = 0;   // RD1 como salida

    RD0 = 1;      // IN1 = 1
    RD1 = 0;      // IN2 = 0
}

//======================================================
// PROGRAMA PRINCIPAL
//======================================================
void main()
{
    unsigned int lecturaADC;
    unsigned int dutyPWM;

    ADC_Init();
    PWM_Init();
    Motor_Init();

    while(1)
    {
        lecturaADC = ADC_Read();  

        // Convertir lectura ADC de 0-1023 a PWM de 0-499
        dutyPWM = ((unsigned long)lecturaADC * 499) / 1023;

        PWM_SetDuty(dutyPWM);

        __delay_ms(10);
    }
}

