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
#include "MLX90614.h"
#include "USART.h"

#define SWITCH  RB1
#define ALARMA  RB2
#define ALUMBRADO  RB3


void init(void);
void read(float LUZ);
void read1(int m);

int a;
float S_Luz;


void main(void) {
    init();
    LCD_INIT();
    T1CON = 0x10;               //Initialize Timer Module
    while(1){    
        Temp = Leer_Sensor(_AMB_TEMP);         // Lee Temp. ambiente
        Temp = (Temp * 0.02) - 273.15;         // Convierte a grados Celsius
        __delay_ms(200);
        
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x61);
        S_Luz = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
//        
//         //I2C_Master_Start();
//        I2C_Master_RepeatedStart();
//        I2C_Master_Write(0x41);
//        S_NIVEL = I2C_Master_Read(0);
////        I2C_Master_Write(0x0F);
//        I2C_Master_Stop();
//        __delay_ms(200);
//       
//        I2C_Master_Start();
//        I2C_Master_Write(0x51);
//        S_CONT = I2C_Master_Read(0);
//        I2C_Master_Stop();
//        __delay_ms(200);
//        PORTB++; 

    //////////////////SENSOR ULTRASONICO/////////////////////////

        TMR1H = 0;                //Sets the Initial Value of Timer
        TMR1L = 0;                //Sets the Initial Value of Timer

        RB0 = 1;                  //TRIGGER HIGH
        __delay_us(10);           //10uS Delay 
        RB0 = 0;                  //TRIGGER LOW

         while(!RB4);              //Waiting for Echo
        TMR1ON = 1;               //Timer Starts
        while(RB4);               //Waiting for Echo goes LOW
        TMR1ON = 0;               //Timer Stops

        a = (TMR1L | (TMR1H<<8)); //Reads Timer Value
        a = a/58;              //Converts Time to Distance
    ///////////////////////////////////////////////////////////////   
    

    ////////////////  DESPLIEGUE EN LCD  //////////////////////////

        Lcd_Clear();
        LCD_XY(0,2);
        LCD_Cadena("S1:  S2:  S3:"); 
        Mostrar_Temperatura(_AMB_TEMP, Temp);  // Muestra los datos 
        S_Luz = (S_Luz/255)*100;
        read(S_Luz);
        read1(a);
    ////////////////////////////////////////////////////////////////
        
        if (Temp>20){
            SWITCH =1;
        }
        else{
            SWITCH =0;
        }
        
        if (a<12){
            ALARMA =1;
        }
        else{
            ALARMA =0;
        }
        if (S_Luz<75){
            ALUMBRADO =1;
        }
        else{
            ALUMBRADO =0;
        }

        
  //      LCD_XY(1,12);
   //     LCD_Cadena(SN);//manda el dato a la LCD
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
    TRISB =0b00011110;     
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

void read(float LUZ){
    char SA[5],SB[5],SC[5],PUNTO1[5];
    int A_,B_,C_,NUM1_;
    NUM1_ =(LUZ)*10;
    A_ = NUM1_%10;   //8
    itoa(SA,A_,10);
    B_ = (NUM1_/10)%10;//9
    itoa(SB,B_,10);
    C_ = (NUM1_/100)%10;//1 
    itoa(SC,C_,10);
    strcpy(PUNTO1,"."); //se coloca el caracter dentro del char
    strcat(PUNTO1,SA); //se concatenan ambos chars
    strcat(SB,PUNTO1); //se concatenan ambos chars
    strcat(SC,SB);
    LCD_XY(1,7);
    LCD_Cadena(SC);//manda el dato a la LCD
}

void read1(int m){
    char SC[5];
    sprintf(SC,"%3u",m);
    LCD_XY(1,12);   
    LCD_Cadena(SC);//manda el dato a la LCD

}
