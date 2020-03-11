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

void main(void) {
    init();
    CONF_SPI();
    _RX_TX();

    while (1){ 

        PORTAbits.RA4 = 0;       //Slave Select
        SPIWRITE(25); //ENVIAR EL DATO
        SPIREAD(); //LEEMOS EL DATO DEL ESCLAVO  
        
        PORTAbits.RA4 = 1;       //Slave Deselect 
        __delay_ms(1);
 
    
    }
    return;     
}

void __interrupt() isr(void){

    if (PIR1bits.RCIF ==1){
        if(RCSTAbits.OERR ==1){
            RCSTAbits.OERR =0;
            __delay_us(10);
        }
        else {
            DEC_INC = RCREG; //ASIGNAMOS EL DATO SERIAL
            PORTD = DEC_INC; //ASIGNAMOS EL DATO SERIAL AL PORTD
        }
    }
    __delay_ms(1);
    PORTAbits.RA4 = 0;       //Slave Select
    SPIWRITE(25); //ENVIAR EL DATO
    SPIREAD();
 
    SerialSendChar(RANDOM);   
    __delay_ms(1);
    PORTAbits.RA4 = 1;       //Slave Deselect 
 
    


}

void init(void) { 
    ///8MH
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    TRISAbits.TRISA5= 0;
    PORTAbits.RA5 =0;
    PORTA =0;
    ///////////////////////////////////////////////////////////
    TRISD =0b00000000; //se define el puerto D como salidas
    //////////////////////////////////////////////////////////////
    TRISCbits.TRISC6 =0; //Tx como salida
    TRISCbits.TRISC7 =1; //Rx como entrada
    PORTD =0;          //se limpia el puerto D
    ANSELH =0;
    ANSEL=0;
    
   
}
