/*
 * File:   main.c
 * Author: debor
 *
 * Created on February 26, 2020, 3:17 PM
 */
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


///MASTER
#define _XTAL_FREQ   8000000

#include <xc.h>
#include <pic16F887.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LCD.h"
#include "I2C.h"

void init(void);


uint8_t S_TEMPERATURA, S_HUMEDAD,S_NIVEL;
char ST[5],SH[5],SN[5];

void main(void) {
    init();
    LCD_INIT();
    
    while(1){
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        S_TEMPERATURA = I2C_Master_Read(0);
//        I2C_Master_Write(0x0F);
        I2C_Master_Stop();
        __delay_ms(200);
        
        //I2C_Master_Start();
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x61);
        S_HUMEDAD = I2C_Master_Read(0);
//        I2C_Master_Write(0x0F);
        I2C_Master_Stop();
        __delay_ms(200);
        
         //I2C_Master_Start();
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x41);
        S_NIVEL = I2C_Master_Read(0);
//        I2C_Master_Write(0x0F);
        I2C_Master_Stop();
        __delay_ms(200);
//       
//        I2C_Master_Start();
//        I2C_Master_Write(0x51);
//        S_CONT = I2C_Master_Read(0);
//        I2C_Master_Stop();
//        __delay_ms(200);
//        PORTB++; 
        
        itoa(ST,S_TEMPERATURA,10);
        itoa(SH,S_HUMEDAD,10);
        itoa(SN,S_NIVEL,10);
        Lcd_Clear();
        LCD_XY(0,2);
        LCD_Cadena("S1:  S2:  S3:"); 
        LCD_XY(1,2);
        LCD_Cadena(ST);//manda el dato a la LCD
        LCD_XY(1,7);
        LCD_Cadena(SH);//manda el dato a la LCD
        LCD_XY(1,12);
        LCD_Cadena(SN);//manda el dato a la LCD
//
//    
    }
    return;
}

void init(void) { 
    ///8MH
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1; 
    ///////////////////////////////////////////////////////////
    TRISD =0b00000000; //se define el puerto D como salidas
    //////////////////////////////////////////////////////////////
    PORTC =0;           //se limpia el puerto C
    PORTD =0;          //se limpia el puerto D
    //////////////////////////////////////////////////////////
    ANSEL = 0;
    ANSELH =0;
    //*****************************************************************************
    // Función de Inicialización
    //***************************************************************************** 
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}