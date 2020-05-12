/*
 * File:   main.c
 * Author: debor
 *
 * Created on March 11, 2020, 2:41 PM
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
#include "I2C.h"
#include "ADC.h"

void init(void);

uint8_t z;
uint8_t dato;
uint8_t S_HUMEDAD,S_NIVEL,S_LLUVIA;

char VARIABLE = 0;

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
         //   PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
          //  __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            if (VARIABLE==0){
            SSPBUF = V_NIVEL;
            VARIABLE=1;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
            PIR1bits.SSPIF = 0;
            return; 
            }
            if (VARIABLE==1){
            SSPBUF = V_HUMEDAD;
            VARIABLE=0;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
            PIR1bits.SSPIF = 0;
            return; 
            }
//            if (VARIABLE==2){
//                SSPBUF = V_LLUVIA;
//                VARIABLE=0;
//                SSPCONbits.CKP = 1;
//                __delay_us(250);
//                while(SSPSTATbits.BF);
//                PIR1bits.SSPIF = 0;
//                return; 
//            }
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

void main(void) {
    init();
    while(1){
        CONF_ADC();
      
    
    }
    return;
}

void init(void) { 
    ///8MH
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1; 
    ///////////////////////////////////////////////////////////
    TRISAbits.TRISA0 =1; //POTENCIOMETRO 0 
    TRISAbits.TRISA1 =1; //POTENCIOMETRO 0 
    TRISAbits.TRISA2 =1; //POTENCIOMETRO 0 
    //////////////////////////////////////////////////////////////
    PORTC =0;            //se limpia el puerto C
    PORTA =0;          //se limpia el puerto A
    //////////////////////////////////////////////////////////
    ANSEL = 0;
    ANSELH =0;
    ANSELbits.ANS0 =1; //RA0 ANALóGICO
    ANSELbits.ANS1 =1; //RA1 ANALóGICO
    ANSELbits.ANS2 =1; //RA2 ANALóGICO
    
    I2C_Slave_Init(0x80); 
}