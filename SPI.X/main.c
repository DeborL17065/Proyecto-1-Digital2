/*
 * File:   main.c
 * Author: debor
 *
 * Created on March 11, 2020, 2:33 PM
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
#include "SPI.h"
#include "USART.h"

void init(void);

int DEC_INC ;
int S =0;

int S_HUMEDAD,S_NIVEL,S_LLUVIA,S_LUZ,S_TEMPERATURA,S_TEMPERATURA1, S_ULTRASONICO;

void __interrupt() isr(void){
     
    if (PIR1bits.SSPIF == 1){
        SPIREAD();
        if(RANDOM ==0x51){
            SPIWRITE(S_TEMPERATURA);
            SSPSTATbits.BF =0;
            PIR1bits.SSPIF =0;
          //  return;
        }
        SPIREAD();
        if(RANDOM ==0x50){
            SPIWRITE(S_LUZ); //Luz
            //PORTDbits.RD0 =1;
            //i=1;
            SSPSTATbits.BF =0;
            PIR1bits.SSPIF =0;
          //  return;
        }
        SPIREAD();
        if(RANDOM ==0x52){
            SPIWRITE(S_HUMEDAD); //Humedad
            //PORTDbits.RD0 =1;
            //i=1;
            SSPSTATbits.BF =0;
            PIR1bits.SSPIF =0;
         //   return;
        }
        SPIREAD();
        if(RANDOM ==0x53){
            SPIWRITE(S_LLUVIA);
            //PORTDbits.RD0 =1;
            //i=1;
            SSPSTATbits.BF =0;
            PIR1bits.SSPIF =0;
         //   return;
        }
        SPIREAD();
        if(RANDOM ==0x54){
            SPIWRITE(S_NIVEL);
            //PORTDbits.RD0 =1;
            //i=1;
            SSPSTATbits.BF =0;
            PIR1bits.SSPIF =0;
         //   return;
        }
        SPIREAD();
        if(RANDOM ==0x55){
            SPIWRITE(S_ULTRASONICO);
            //PORTDbits.RD0 =1;
            //i=1;
            SSPSTATbits.BF =0;
            PIR1bits.SSPIF =0;
         //   return;
        }
        SSPSTATbits.BF =0;
        PIR1bits.SSPIF =0;
        return;
    // return;
    }
    
    if (PIR1bits.RCIF ==1 ){
        
        if(S==0){
            S_TEMPERATURA = SERIALREAD();
            PIR1bits.RCIF =0;
            PORTDbits.RD0 =0;
            S=1;
           // return;
        }
        
        if(S==1){
            S_LUZ = SERIALREAD();
            PIR1bits.RCIF =0;
            PORTDbits.RD0 =1;
            S=2;
           // return;
        }
        if(S==2){
            S_HUMEDAD = SERIALREAD();
            PIR1bits.RCIF =0;
            PORTDbits.RD0 =0;
            S=3;
          //  return;
        }
        if(S==3){
            S_LLUVIA = SERIALREAD();
            PIR1bits.RCIF =0;
            PORTDbits.RD0 =0;
            S=4;
          //  return;
        }
        if(S==4){
            S_NIVEL = SERIALREAD();
            PIR1bits.RCIF =0;
            PORTDbits.RD0 =0;
            S=5;
          //  return;
        }
        if(S==5){
            S_ULTRASONICO = SERIALREAD();
            PIR1bits.RCIF =0;
            PORTDbits.RD0 =0;
            S=0;
          //  return;
        }
        PIR1bits.RCIF =0;
        return;

    }
    

 

}

void main(void) {
    init();
    CONF_SPI();
    _RX_TX();
    while (1){   
        
        SerialSendChar(S_TEMPERATURA);
        __delay_ms(1);
        SerialSendChar(S_LUZ);
        __delay_ms(1);
        SerialSendChar(S_HUMEDAD);
        __delay_ms(1);
        SerialSendChar(S_LLUVIA);
        __delay_ms(1);
        SerialSendChar(S_NIVEL);
        __delay_ms(1);
        SerialSendChar(S_ULTRASONICO);
        __delay_ms(1);
//       
       

//        PORTAbits.RA4 = 0;       //Slave Select
//        SPIWRITE(25); //ENVIAR EL DATO
//        SPIREAD(); //LEEMOS EL DATO DEL ESCLAVO  
//        
//        PORTAbits.RA4 = 1;       //Slave select 
//        __delay_ms(1);
// 
    
    }
    return;     
}



void init(void) { 
    ///8MH
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    ////////////////////////////////////////////////////////////
    //TRISAbits.TRISA5= 1;
    PORTAbits.RA5 =0;
    PORTA =0;
    ///////////////////////////////////////////////////////////
  //  TRISD =0b00000000; //se define el puerto D como salidas
    //////////////////////////////////////////////////////////////
    TRISCbits.TRISC6 =0; //Tx como salida
    TRISCbits.TRISC7 =1; //Rx como entrada
    TRISDbits.TRISD0 =0;
    PORTD =0;          //se limpia el puerto D
    ANSELH =0;
    ANSEL=0;
    
   
}
