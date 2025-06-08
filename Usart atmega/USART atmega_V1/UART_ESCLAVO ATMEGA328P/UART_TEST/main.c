/*
 * UART_TEST.c
 *
 * Created: 24/1/2025 05:03:00
 * Author : Aleman
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Commands.h"
#include "lcd.h"
#include "usart.h"
//Esclavo Test

#define Trama 0x04
void Procesar_Trama(void);

char str[5];

int main(void)
{
	cli();
	Usart_init();
	sei();
	lcd_init();
	
    /* Replace with your application code */
    while (1)
    {
	Usart_Guardar_Valor_Buffer();
	//Procesar_Trama();
    }
}

void Procesar_Trama(void){
	if(Usart_Trama_Disponible() == true){
		if(Usart_Trama.comando == Trama){
		}
	Usart_Reinicio_Trama();
	}
}

