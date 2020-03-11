/*
 * File:   MLX90614.c
 * Author: debor
 *
 * Created on March 1, 2020, 9:57 PM
 */


#include <xc.h>
#include <pic16F887.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MLX90614.h"
#include "LCD.h"
#include "I2C.h"

/*****************************************************************************
* Función para leer el sensor por I2C.
* Vectores de Lectura  
* 0x06 Para temperatura ambiente
* 0x07 Para la temperatura del objeto
* Retorna el valor de temperatura en grados Kelvin.
*****************************************************************************/
float Leer_Sensor(char Temp_Source){
    
    unsigned int Temp_var;
    
    I2C_Master_Start();
    I2C_Master_Write(_IR_THERMO_ADDR << 1);
    I2C_Master_Write(Temp_Source);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(_IR_THERMO_ADDR << 1);
    Temp_var = I2C_Master_Read(0);
    Temp_var = (I2C_Master_Read(0) << 8) + Temp_var;
    I2C_Master_Stop();                  
 
  return Temp_var;
}

/*****************************************************************************
* Muestra los valores de temperatura tanto del objeto como el ambiente.
*****************************************************************************/
void Mostrar_Temperatura(char Temp_Source, float temperatura){
    
    char text[5],SA[5],SB[5],SC[5],PUNTO1[5];
 
    if (Temp_Source == _AMB_TEMP){
        LCD_XY(1,2);
        NUM1 = (temperatura)*10;
        A = NUM1%10;   //8
        itoa(SA,A,10);
        B = (NUM1/10)%10;//9
        itoa(SB,B,10);
        C = (NUM1/100)%10;//1 
        itoa(SC,C,10);
        strcpy(PUNTO1,"."); //se coloca el caracter dentro del char
        strcat(PUNTO1,SA); //se concatenan ambos chars
        strcat(SB,PUNTO1); //se concatenan ambos chars
        strcat(SC,SB); //se concatenan ambos chars
        LCD_Cadena(SC);//manda el dato a la LCD
    }
    if (Temp_Source == _OBJ_TEMP){
        LCD_XY(1,2);
        LCD_Cadena(text);//manda el dato a la LCD
    }
}
