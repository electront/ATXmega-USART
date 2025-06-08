/*
 * UART_TEST.c
 *
 * Created: 24/1/2025 05:03:00
 * Author : Aleman
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "Commands.h"
#include "usart.h"
int count = 0;
//Master Test
int main(void)
{
	cli();
	Usart_init();
	
	
	sei();
    /* Replace with your application code */
    while (1)
    {
		Usart_Enviar_Comando(EQUIPO_ON);
		_delay_ms(1000);
		Usart_Enviar_Comando(EQUIPO_OFF);
		_delay_ms(1000);
    }
}

