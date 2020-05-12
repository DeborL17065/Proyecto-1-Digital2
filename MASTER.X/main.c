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
#define SWITCH_AGUA RB5
#define SWITCH_COMPUERTA RA0
#define ALARMA  RB2
#define ALUMBRADO  RB3
#define PULSADOR   RE2


void init(void);
void read(float LUZ);
void read3(float LUZ);
void read2(float LUZ);
void read4(float LUZ);
void read1(int m);
float S_HUMEDAD,S_NIVEL,S_LLUVIA;
uint8_t S_ALARMA;
uint8_t BOTON=0;
uint8_t Estado1 =0;
int a=0;
float  S_Luz;


void main(void) {
    init();
    _RX_TX();
    LCD_INIT();
    T1CON = 0x10;                              //Initialize Timer Module
    while(1){
       
        Leer_Sensor();
        Temp = Temp_var;                        // Lee Temp. ambiente
        Temp = (Temp * 0.02) - 273.15;         // Convierte a grados Celsius
        __delay_ms(200);
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        S_Luz = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
         
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x81);
        S_NIVEL = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x71);
        S_HUMEDAD = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0x81);
        S_LLUVIA = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        

        
    //////////////////SENSOR DE HUMEDAD ///////////////////////  
        S_HUMEDAD = (255 - S_HUMEDAD);
        S_LLUVIA = (255 - S_LLUVIA);
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
        S_HUMEDAD = (S_HUMEDAD/255)*100;
        S_Luz = (S_Luz/255)*100;
        S_NIVEL = (S_NIVEL/255)*100;
        S_LLUVIA = (S_LLUVIA/255)*100;
        
        if ((S_NIVEL<90) && (S_LLUVIA!=0)){
            SWITCH_COMPUERTA =1;
        }
        else{
            SWITCH_COMPUERTA =0;
        } 
        if ((S_HUMEDAD<50) && (S_NIVEL>20)){
            SWITCH_AGUA =1;
        }
        else{
            SWITCH_AGUA =0;
        }  
        if (Temp>20){
            SWITCH =1;
        }
        else{
            SWITCH =0;
        }   
        if (a<12){
          //  ALUMBRADO=1;
            ALARMA =1;
            __delay_ms(10);
         //   ALUMBRADO=0;
            ALARMA =0;
            __delay_ms(10);
            S_ALARMA =1;
         
        }
        else{
            ALARMA =0;
            S_ALARMA =0;
         
        }
        if (S_Luz>75){
            ALUMBRADO =1;
        }
        else{
            ALUMBRADO =0;
        }
 ////////////////  DESPLIEGUE EN LCD  //////////////////////////
        if (PULSADOR ==1){   
            Estado1 =1;
        }
        if (Estado1 ==1 && PULSADOR ==0){ //boton con antirebote
            Estado1 =0;
            BOTON++ ;      //se incrementa el contador
        }  
        if (BOTON ==0){
            Lcd_Clear();
            LCD_XY(0,0);
            LCD_Cadena("TEMP:  LUZ:  ALA:"); 
            Mostrar_Temperatura(_AMB_TEMP, Temp);  // Muestra los datos 
            read3(S_Luz);
            read1(a);
        
        }
        if (BOTON ==1){
      
            Lcd_Clear();
            LCD_XY(0,0);
            LCD_Cadena("HUM:  NIV:  LLU:"); 
            read2(S_HUMEDAD);
            read(S_NIVEL);
            read4(S_LLUVIA);
        
        }
        if (BOTON ==2){ 
            BOTON =0;
        }
/////////////////////////////////////////////////////////////// 
        Temp = (Temp/100)*255;
        S_Luz = (S_Luz/100)*255;
        S_HUMEDAD = (S_HUMEDAD/100)*255;
        S_LLUVIA = (S_LLUVIA/100)*255;
        S_NIVEL = (S_NIVEL/100)*255;

        SerialSendChar(Temp); // Temp
        __delay_ms(1);
        SerialSendChar(S_Luz);
        __delay_ms(1);
        SerialSendChar(S_HUMEDAD); ///Humedad
        __delay_ms(1);
        SerialSendChar(S_LLUVIA); ///Lluvia
        __delay_ms(1);
        SerialSendChar(S_NIVEL); //nivel
        __delay_ms(1);
        SerialSendChar(Temp); //alarma
        __delay_ms(1);
  ////////////////////////////////////////////////////////////////          
    }
    return;
}

void init(void) { 
    ///8MH
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1; 
    ///////////////////////////////////////////////////////////
    
    TRISEbits.TRISE2 =1; // entrada  
    TRISD =0b00000000; //se define el puerto D como salidas
    //TRISB =0b0001000; 
    TRISBbits.TRISB0 =0;
    TRISBbits.TRISB4 =1;
    TRISBbits.TRISB2 =0;
    TRISBbits.TRISB3 =0;
    TRISBbits.TRISB5 =0;
    TRISBbits.TRISB1 =0;
    TRISAbits.TRISA0 =0; //Tx como salida
    TRISCbits.TRISC6 =0; //Tx como salida
    TRISCbits.TRISC7 =1; //Rx como entrada  
    //////////////////////////////////////////////////////////////
    PORTC =0;           //se limpia el puerto C
    PORTD =0;          //se limpia el puerto D
    PORTEbits.RE2 =0;
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
    LCD_XY(1,6);
    LCD_Cadena(SC);//manda el dato a la LCD
}
void read3(float LUZ){
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

void read2(float LUZ){
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
    LCD_XY(1,0);
    LCD_Cadena(SC);//manda el dato a la LCD
}

void read4(float LUZ){
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
    LCD_XY(1,12);
    LCD_Cadena(SC);//manda el dato a la LCD
}
void read1(int m){
    char SC[5];
    sprintf(SC,"%3u",m);
    LCD_XY(1,12);   
    LCD_Cadena(SC);//manda el dato a la LCD
}
