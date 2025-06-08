/*
 * UART_TEST.c
 *
 * Created: 24/1/2025 05:03:00
 * Author : Aleman
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Commands.h"
#include "lcd.h"
#include "usart.h"
//Esclavo Test
/*
Notas:
para mostrar el valor que manda los bit q salen en hexadecimal poner para el lcd "%d"

*/
#define Byte 10


void Procesar_Trama(void);
int count =0;
char str[5];

int main(void)
{
	cli();
	Usart_init();
	lcd_init();
	sei();
    /* Replace with your application code */
    while (1)
    {	
		
	Usart_Guardar_Valor_Buffer();
	Procesar_Trama();
	
    }
}

void Procesar_Trama(void){
	if(Usart_Trama.trama_disponible == true){
		if(Usart_Trama.comando == ENVIO_COMANDO){
			lcd_gotorc(1,12);
			lcd_puts("cmd ok");
		}
		if(Usart_Trama.dato_1_byte == Byte){
			lcd_gotorc(1,12);
			lcd_puts("dato  ");
			
			sprintf(str, "%i", ++count);
			lcd_gotorc(1,19);
			lcd_puts(str);
		}
	Usart_Reinicio_Trama();
	}
}


