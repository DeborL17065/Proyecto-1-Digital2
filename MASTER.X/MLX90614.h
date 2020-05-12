/* 
 * File:   MLX90614.h
 * Author: debor
 *
 * Created on March 1, 2020, 9:59 PM
 */

#ifndef MLX90614_H
#define	MLX90614_H
#define _XTAL_FREQ   8000000

int _IR_THERMO_ADDR = 0x5A;
int _AMB_TEMP      = 0x06;
int _OBJ_TEMP      = 0x07;
int A,B,C,NUM1;
float Temp;
unsigned int Temp_var;
uint8_t Temp_var1,Temp_var2;
float Leer_Sensor();
void Mostrar_Temperatura(char Temp_Source, float temperatura);
#endif	/* MLX90614_H */

