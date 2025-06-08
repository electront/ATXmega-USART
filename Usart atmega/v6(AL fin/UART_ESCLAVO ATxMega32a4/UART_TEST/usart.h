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
#include <math.h>
#include "Buffer.h"
#include "Commands.h"

#define USART_PC 0
#define USART_MICRO 1

#define BUFFER_PC 0
#define BUFFER_MICRO 1

struct Usart_Trama_Struct{
	uint8_t  Count;
	uint8_t  tamano_trama;
	uint8_t  comando;
	uint8_t  dato_1_byte;
	uint16_t dato_2_byte;
	uint16_t checksum;

	volatile bool trama_disponible;
	
}Usart_Trama_Micro;



void Usart_Inicializar();

void Usart_Inicializar_Buffer();

void Usart_Reinicio_Trama();

void Usart_Procesar_Datos();


#endif /* USART_H_ */