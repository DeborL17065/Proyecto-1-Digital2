/*
 * File:   USART.c
 * Author: debor
 *
 * Created on February 11, 2020, 11:29 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "USART.h"

void _RX_TX (void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    TXSTAbits.TX9  =0;
    RCSTAbits.RX9  =0;
    
    BAUDCTLbits.BRG16 =0; //BIT PARA LA VELOCIDAD
    TXSTAbits.BRGH =1; //BIT PARA LA VELOCIDAD
    TXSTAbits.SYNC =0; //TX/RX ASINCRONAS
    RCSTAbits.SPEN =1; //ACTIVAR EL PUERTO SERIE
    RCSTAbits.CREN =1; //ACTIVAR LA RECEPCION
    SPBRG =51; //BAUDRATE 9600
    TXSTAbits.TXEN =1; //ACTIVAR LA INTERRUPCION POR TRANSMISION 
    PIE1bits.RCIE =1;  //ACTIVAR LA INTERRUPCION POR RECEPCION 
   // PIE1bits.TXIE =1;  //ACTIVAR LA INTERRUPCION POR RECEPCION
    
    PIR1bits.TXIF =0; //limpiamos la bandera de TX
    PIR1bits.RCIF =0; //lipiamos la bandera de RX
    SPBRGH =0;   
}


void SerialSendChar (char CARACTER) {
    TXREG = CARACTER; //EL REGISTRO DE TX SE CARGA CON "CARACTER"
    while(!TXSTAbits.TRMT); 
    return;
}

void SerialSendString(char *txbuffer){
    unsigned int l = strlen(txbuffer);
    for (unsigned int i=0;i<l;i++){
        TXREG =*txbuffer++; //SE ENVIA CARACTER POR CARACTER
        while(!TXSTAbits.TRMT);
    
    }
    

}