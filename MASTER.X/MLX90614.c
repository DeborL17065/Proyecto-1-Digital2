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
   // I2C_Master_RepeatedStart();
    I2C_Master_Write(_IR_THERMO_ADDR << 1);
    I2C_Master_Write(Temp_Source);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(_IR_THERMO_ADDR << 1);
    Temp_var = I2C_Master_Read(0);
    Temp_var = (I2C_Master_Read(0) << 8) + Temp_var;
    //        I2C_Master_Write(0x0F);
    I2C_Master_Stop();                  
 
  return Temp_var;
}

/*****************************************************************************
* Muestra los valores de temperatura tanto del objeto como el ambiente.
*****************************************************************************/
void Mostrar_Temperatura(char Temp_Source, float temperatura){
    
    char text[15];
  //sprintf(text,"%2.1f", temperatura);
 
    if (Temp_Source == _AMB_TEMP){
        LCD_XY(1,2);
        LCD_Cadena(text);//manda el dato a la LCD
//        Lcd_Out(2, 2, text);
//        Lcd_Chr(2,6,223);
//        Lcd_Chr(2,7,0x43);
    }
    if (Temp_Source == _OBJ_TEMP){
        LCD_XY(1,2);
        LCD_Cadena(text);//manda el dato a la LCD
//        Lcd_Out(2, 10, text);
//        Lcd_Chr(2,14,223);
//        Lcd_Chr(2,15,0x43);
    }
}
