#include <xc.h>
#include <stdbool.h>
#include <stdio.h>
#include "lcd.h"
#include "keypad.h"
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 8000000

void main(void){
    LCD lcd = {&PORTC,2,3,4,5,6,7};
    char tecla;
    char operador = 0;
    float num1 = 0;
    float num2 = 0;
    float resultado = 0;
    bool capturando_num2 = false;
    char buffer[17];
    ANSEL = 0x00;
    ANSELH = 0x00;
    TRISC = 0x00;


    LCD_Init(lcd);
    InitKeypad();
    
    while(1){
        tecla = switch_press_scan();
        if(tecla == '*'){
            num1 = 0;
            num2 = 0;
            resultado = 0;

            operador = 0;

            capturando_num2 = false;

            LCD_Clear();
        }

        else if(tecla >= '0' && tecla <= '9')
        {
            LCD_putc(tecla);

            if(capturando_num2 == false)
            {
                num1 = num1 * 10 + (tecla - '0');
            }
            else
            {
                num2 = num2 * 10 + (tecla - '0');
            }
        }

        //------------------------------------------------
        // OPERADORES
        //------------------------------------------------

        else if(tecla=='A' || tecla=='B' ||
                tecla=='C' || tecla=='D')
        {
            capturando_num2 = true;
            switch(tecla){
                case 'A':
                    operador = '+';
                    LCD_putc('+');
                    break;
                case 'B':
                    operador = '-';
                    LCD_putc('-');
                    break;
                case 'C':
                    operador = '*';
                    LCD_putc('*');
                    break;
                case 'D':
                    operador = '/';
                    LCD_putc('/');
                    break;
            }
        }

        else if(tecla == '#'){
            switch(operador){
                case '+':
                    resultado = num1 + num2;
                    break;

                case '-':
                    resultado = num1 - num2;
                    break;

                case '*':
                    resultado = num1 * num2;
                    break;

                case '/':
                    if(num2 != 0)
                    {
                        resultado = num1 / num2;
                    }
                    else
                    {
                        LCD_Clear();
                        LCD_puts("Error Div0");

                        __delay_ms(1500);

                        LCD_Clear();

                        num1 = 0;
                        num2 = 0;
                        operador = 0;
                        capturando_num2 = false;

                        continue;
                    }
                    break;
            }

            LCD_Clear();

            sprintf(buffer,"%.2f",resultado);

            LCD_puts(buffer);

            num1 = resultado;
            num2 = 0;
            operador = 0;
            capturando_num2 = false;
        }
    }
}



