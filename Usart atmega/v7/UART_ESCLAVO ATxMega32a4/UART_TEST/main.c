/*
 * UART_TEST.c
 *
 * Created: 24/1/2025 05:03:00
 * Author : Aleman
 */ 

#define F_CPU 2000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Commands.h"
#include "ATXMega_lcd.h"
#include "usart.h"
//Esclavo Test
/*
Notas:
para mostrar el valor que manda los bit q salen en hexadecimal poner para el lcd "%d"

*/

#define Byte1 1
#define Byte2 2
#define Byte3 3
#define Byte4 4
#define Byte5 5

#define C_Encender 0x03

void Procesar_Trama(void);
char str[5];


ISR(USARTC1_RXC_vect){
	uint8_t data = USARTC1.DATA; // Leer dato recibido
	
	if(!Buffer_Esta_Lleno()){ // Reviso si el buffer está lleno
		Buffer_Add(data);	 // Escribir en el buffer
	}
}

int main(void)
{
	cli();
	Usart_Inicializar(USART_MICRO);
	Usart_Inicializar(USART_PC);
	lcd_init();
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	sei();
	
    /* Replace with your application code */
    while (1)
    {	
	Usart_Procesar_Datos();
	
	Procesar_Trama();
	
    }
}


void Procesar_Trama(void){
	if(Usart_Trama_Micro.trama_disponible == true){
		if(Usart_Trama_Micro.comando == C_Encender){
			lcd_gotorc(1,12);
			lcd_puts("C_ok");
		}
		if(Usart_Trama_Micro.dato_1_byte == Byte1){
			lcd_gotorc(1,12);
			lcd_puts("Dato:");
			
			sprintf(str, "%i", Usart_Trama_Micro.dato_1_byte);
			lcd_gotorc(1,18);
			lcd_puts(str);
		}
		else if(Usart_Trama_Micro.dato_1_byte == Byte2){
			lcd_gotorc(1,12);
			lcd_puts("Dato:");
			
			sprintf(str, "%i", Usart_Trama_Micro.dato_1_byte);
			lcd_gotorc(1,18);
			lcd_puts(str);
		}
		else if(Usart_Trama_Micro.dato_1_byte == Byte3){
			lcd_gotorc(1,12);
			lcd_puts("Dato:");
			
			sprintf(str, "%i", Usart_Trama_Micro.dato_1_byte);
			lcd_gotorc(1,18);
			lcd_puts(str);
		}
		else if(Usart_Trama_Micro.dato_1_byte == Byte4){
			lcd_gotorc(1,12);
			lcd_puts("Dato:");
			
			sprintf(str, "%i", Usart_Trama_Micro.dato_1_byte);
			lcd_gotorc(1,18);
			lcd_puts(str);
		}
		//else if(Usart_Trama_Micro.dato_1_byte == Byte5){
			lcd_gotorc(1,12);
			lcd_puts("Dato:");
			
			sprintf(str, "%i", Usart_Trama_Micro.dato_1_byte);
			lcd_gotorc(1,18);
			lcd_puts(str);
		//}
	Usart_Reinicio_Trama(BUFFER_MICRO);
	}
}


