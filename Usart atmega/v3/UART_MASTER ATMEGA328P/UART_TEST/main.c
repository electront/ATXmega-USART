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


#define C_Encender 'e'

int byte = 0;

void Enviar_Trama();


//Master Test
int main(void)
{
	cli();
	Usart_init();
	sei();

	
	Enviar_Trama();
    while (1)
    {
	
    }
}


void Enviar_Trama(){ 	
	Usart_Enviar_Dato(COMIENZO_TRAMA);
	Usart_Enviar_Dato(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Dato(C_Encender);
	Usart_Enviar_Dato(byte);
	Usart_Enviar_Dato(0x02+C_Encender+byte);	//checksum
}


