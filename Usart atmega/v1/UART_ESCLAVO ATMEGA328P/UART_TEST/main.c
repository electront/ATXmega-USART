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
#include "lcd.h"
#include "usart.h"
int count = 0;
//Esclavo Test

void Procesar_Trama(void);



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
	if(Usart_Trama_Disponible() == true){
		if(Usart_Trama.comando == EQUIPO_ON){
			lcd_clear();
			lcd_gotorc(1,1);
			lcd_puts("Equipo ON");
		}
		else if(Usart_Trama.comando == EQUIPO_OFF){
			lcd_clear();
			lcd_gotorc(1,1);
			lcd_puts("Equipo OFF");
		}
	Usart_Reinicio_Trama();
	}
}