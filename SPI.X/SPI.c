/*
 * File:   SPI.c
 * Author: debor
 *
 * Created on February 12, 2020, 12:01 AM
 */

///SLAVE
#define _XTAL_FREQ   8000000
#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "SPI.h"

void CONF_SPI(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    ///////////////////////////////////////////////////////
    TRISCbits.TRISC5 =0; //SDO SALIDA DE DATOS
    TRISCbits.TRISC3 =1; //SCK ENTRADA DEL RELOJ
    TRISCbits.TRISC4 =1; //SDI ENTRADA DE DATOS
    TRISAbits.TRISA5 =1; //SS ENTRADA PARA EL ESCLAVO
    //-------CONFIGURACION SSPSTAT---------------------
    SSPSTATbits.SMP = 0; //DEBE SER 0 EN MODO ESCLAVO
    SSPSTATbits.CKE = 0;
    //-------CONFIGURACION SSPCON---------------------
    SSPCONbits.SSPM = 0b0100;
    SSPCONbits.CKP = 1; //ESTADO IDLE DEL RELOJ ES NIVEL ALTO
    SSPCONbits.SSPEN = 1; //ACTIVAR PUERTO SERIAL
    
  //  SSPCONbits.SSPOV = 0; //LIMPIAR BANDERA OVERFLOW
    
    //////////////////////////////////////////////////////////

}

void SPIWRITE (char dato){

        SSPBUF = dato; //ENVIAMOS DATO
   
}

void SPIREAD (void){
    RANDOM = SSPBUF; 
    return;
}

