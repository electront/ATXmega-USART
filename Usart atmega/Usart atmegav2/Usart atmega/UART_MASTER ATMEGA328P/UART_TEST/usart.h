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
#include "buffer.h"
#include "Commands.h"

#if defined(__AVR_ATmega88__) || (__AVR_ATmega88A__) || (__AVR_ATmega88P__) || (__AVR_ATmega88PA__) || (__AVR_ATmega88PB__) || (__AVR_ATmega328P__)
#define __USART_OUTPUT_TX()         (DDRD |= (1 << DDD1))
#define __USART_INPUT_RX()          (DDRD &= ~(1 << DDD0))
#define __USART_ENABLE_TX_RX()      (UCSR0B |= (1 << RXEN0) | (1 << TXEN0))
#define __USART_ENABLE_IRQ_RX()  ( UCSR0B |= (1 << RXCIE0))
#define __USART_MODE_AS_IMP_1STOP() (UCSR0C |= (1 << UPM00) | (1 << UPM01) | (1 << UCSZ00) | (1 << UCSZ01))
#define __USART_BAUD_9600()         (UBRR0 = 103)
#define __USART_BUFFER              UDR0
#define __USART_WAIT()              (UCSR0A & (1 << UDRE0))
#define __USART_IRQ_VECT            USART_RX_vect
#endif


#if defined(__AVR_ATmega2560__)
#define __USART_OUTPUT_TX()         (DDRE |= (1 << DDE1))
#define __USART_INPUT_RX()          (DDRE &= ~(1 << DDE0))
#define __USART_ENABLE_TX_RX()      (UCSR0B |= (1 << RXEN0) | (1 << TXEN0))
#define __USART_ENABLE_IRQ_RX()  (UCSR0B |= (1 << RXCIE0))
#define __USART_MODE_AS_IMP_1STOP() (UCSR0C |= (1 << UPM00) | (1 << UPM01) | (1 << UCSZ00) | (1 << UCSZ01))
#define __USART_BAUD_9600()         (UBRR0 = 51)
#define __USART_BUFFER              UDR0
#define __USART_WAIT()              (UCSR0A & (1 << UDRE0))
#define __USART_IRQ_VECT            USART0_RX_vect
#endif


struct Usart_Buffer_Struct{
	uint8_t  tamano_trama;
	uint8_t  comando;
	uint8_t	 dato_1_byte;
	uint16_t dato_2_byte;
	uint8_t  checksum;
	
	volatile bool	deteccion_comienzo_trama;
	volatile bool	deteccion_tamano_trama;
	volatile bool	deteccion_comando_trama;
	volatile bool	deteccion_checksum;
	volatile bool	trama_disponible;
	
}Usart_Trama;


void Usart_init();

void Usart_Inicializar_Buffer();

void Usart_Reinicio_Trama();

void Usart_Enviar_Hex(uint8_t valor_enviar);

void Usart_Error_Trama();

void Usart_Guardar_Valor_Buffer();

void Usart_Enviar_Fin_Datos();

void Usart_Enviar_Comando(Command comando);

void Usart_Enviar_Sensado(Command comando, uint16_t valor_enviar);

bool Usart_Esta_Enviando_Datos(void);

#endif /* USART_H_ */