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


#define C_Encender 'e'
#define C_PTT 'p'
#define C_Apagar 'a'


void Enviar_Trama(uint8_t Byte);


//Master Test
int main(void)
{
	cli();
	Usart_Inicializar();
	sei();
	
	_delay_ms(500);

	Enviar_Trama(5);



    while (1)
    {
    }
}


void Enviar_Trama(uint8_t Byte){ 	
	Usart_Enviar_Dato(COMIENZO_TRAMA);
	Usart_Enviar_Dato(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Dato(C_Encender);
	Usart_Enviar_Dato(Byte);
	Usart_Enviar_Dato(0x02+C_Encender+Byte);	//checksum
		Usart_Enviar_Dato(COMIENZO_TRAMA);
		Usart_Enviar_Dato(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
		Usart_Enviar_Dato(C_PTT);
		Usart_Enviar_Dato(Byte);
		Usart_Enviar_Dato(0x02+C_PTT+Byte);	//checksum
			Usart_Enviar_Dato(COMIENZO_TRAMA);
			Usart_Enviar_Dato(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
			Usart_Enviar_Dato(C_Apagar);
			Usart_Enviar_Dato(Byte);
			Usart_Enviar_Dato(0x02+C_Apagar+Byte);	//checksum
}


