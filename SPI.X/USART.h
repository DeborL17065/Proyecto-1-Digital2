/* 
 * File:   USART.h
 * Author: debor
 *
 * Created on February 11, 2020, 11:30 PM
 */

#ifndef USART_H
#define	USART_H


void _RX_TX (void);
void SerialSendChar (char CARACTER);
void SerialSendString(char *txbuffer);
uint8_t SERIALREAD();

#endif	/* USART_H */

