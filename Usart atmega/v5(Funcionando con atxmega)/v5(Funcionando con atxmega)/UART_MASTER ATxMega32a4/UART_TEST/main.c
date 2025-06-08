/*
 * UART_TEST.c
 *
 * Created: 24/1/2025 05:03:00
 * Author : Aleman
 */ 

#define F_CPU 2000000
#include <avr/io.h>
#include <util/delay.h>
#include "Commands.h"
#include "usart.h"


#define C_Encender 's'
uint8_t byte = 2;

void Enviar_Trama();


//Master Test
int main(void)
{
	cli();
	Usart_Inicializar();
	sei();
	
	_delay_ms(500);
	
	Enviar_Trama();


    while (1)
    {
    }
}


void Enviar_Trama(){ 
	uint8_t Checksum = 0;
/*	
	Checksum += 0x02;
	Checksum += C_Encender;
	Checksum += dato;*/
		
	Usart_Enviar_Dato(COMIENZO_TRAMA);
	_delay_ms(10);
	Usart_Enviar_Dato(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Dato(0x03);
	Usart_Enviar_Dato(C_Encender);
	Usart_Enviar_Dato(0x02+0x03+C_Encender);	//checksum
}


