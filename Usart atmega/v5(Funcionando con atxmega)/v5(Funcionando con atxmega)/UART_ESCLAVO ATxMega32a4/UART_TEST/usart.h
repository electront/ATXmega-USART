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
	uint8_t  tamano_trama;
	uint8_t  comando;
	uint8_t	 dato_1_byte;
	uint16_t dato_2_byte;
	uint16_t  checksum;
	uint8_t Count;
	
	volatile bool	deteccion_comienzo_trama;
	volatile bool	deteccion_tamano_trama;
	volatile bool	deteccion_comando_trama;
	volatile bool	deteccion_checksum;
	volatile bool	trama_disponible;
	
}Usart_Trama_PC, Usart_Trama_Micro;



void Usart_Inicializar(uint8_t puerto);

void Usart_Inicializar_Buffer(uint8_t buffer);

void Usart_Reinicio_Trama(uint8_t buffer);

void Usart_Enviar_Ascii(uint8_t Puerto, const char *valor_enviar);

void Usart_Enviar_Hex(uint8_t Puerto, uint8_t valor_enviar);

void Usart_Error_Trama(uint8_t Puerto, uint8_t Buffer);

void Usart_Guardar_Valor_Buffer(uint8_t Buffer);

void Usart_Procesar_Datos();

void Usart_Enviar_Fin_Datos();

void Usart_Enviar_Comando(Command comando);

void Usart_Enviar_Sensado(Command comando, uint16_t valor_enviar);

bool Usart_Esta_Enviando_Datos(void);


#endif /* USART_H_ */