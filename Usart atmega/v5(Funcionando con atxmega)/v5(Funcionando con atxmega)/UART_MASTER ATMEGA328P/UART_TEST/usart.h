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


void Usart_init();

void Usart_Enviar_Dato(unsigned char Dato);

#endif /* USART_H_ */