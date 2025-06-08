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


void Enviar_Trama();
void Enviar_Trama1();
void Enviar_Trama2();

//Master Test
int main(void)
{
	cli();
	Usart_init();
	sei();
	
	
	//Enviar_Trama();
	Enviar_Trama2();
    while (1)
    {
	
	//Enviar_Trama1();
	//Enviar_Trama2();
    }
}


void Enviar_Trama(){ 
	uint16_t checksum = 0;
	
	checksum += 0x02;
	checksum += ENVIO_COMANDO;
	checksum += 0x04;
	
	Usart_Enviar_Hex(COMIENZO_TRAMA);
	Usart_Enviar_Hex(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Hex(ENVIO_COMANDO);
	Usart_Enviar_Hex(0x04);
	Usart_Enviar_Hex(checksum);	//checksum
}


void Enviar_Trama1(){
	uint16_t checksum = 0;
	
	checksum += 0x02;
	checksum += ENVIO_COMANDO;
	checksum += 0x05;
	
	Usart_Enviar_Hex(COMIENZO_TRAMA);
	Usart_Enviar_Hex(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Hex(ENVIO_COMANDO);
	Usart_Enviar_Hex(0x05);
	Usart_Enviar_Hex(checksum);	//checksum
}


void Enviar_Trama2(){
	uint16_t checksum = 0;
	
	checksum += 0x02;
	checksum += ENVIO_COMANDO;
	checksum += 0x06;
	
	Usart_Enviar_Hex(COMIENZO_TRAMA);
	Usart_Enviar_Hex(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Hex(ENVIO_COMANDO);
	Usart_Enviar_Hex(0x06);
	Usart_Enviar_Hex(checksum);	//checksum
}

