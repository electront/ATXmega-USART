/*
 * Puerto_Serie_AtxMega.h
 *
 * Created: 27/01/2022 12:46:10
 *  Author: usuario
 */ 


#ifndef USART_H_
#define USART_H_
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "Commands.h"

void Usart_Inicializar();

void Usart_Enviar_Dato(uint8_t Dato);

#endif /* USART_H_ */