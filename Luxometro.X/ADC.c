/*
 * File:   ADC.c
 * Author: debor
 *
 * Created on February 12, 2020, 12:01 AM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ADC.h"


void CONF_ADC(void) {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    ADCON0 = 0b01000001; //AN0
    ADCON1 = 0b00000000;
    ADCON0bits.GO_DONE =1; //se inicia la conversión
    while(ADCON0bits.GO_DONE ==1); 
    SENSOR1 = ADRESH;
    

  
    return;
}
