/*
 * Puerto_Serie_AtxMega.c
 *
 * Created: 27/01/2022 12:45:47
 *  Author: usuario
 */ 


#include "usart.h"
#include "ATXMega_lcd.h"


//Los declaro en el '.c' para que no puedan ser accedidos por nada fuera de
//este archivo.


char letra[5];
int contador = 0;


void Usart_Inicializar(){
	PORTC.DIRSET = PIN7_bm; // TX
	PORTC.DIRCLR = PIN6_bm; // RX
	USARTC1.CTRLA = USART_RXCINTLVL_MED_gc; // Habilitando interrupción por recepción.
	USARTC1.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Habilitando recepción y transmisión.
	USARTC1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	USARTC1.BAUDCTRLA = 0x03; // Baudrate 9600.
	USARTC1.BAUDCTRLB = 0x96;
	Usart_Inicializar_Buffer(); // Inicializar Buffer y Trama.

}


void Usart_Inicializar_Buffer() {
	
	Buffer_Inicializar();

	// Inicializar todos los campos del buffer
	Usart_Trama_Micro.trama_disponible = false;
	Usart_Trama_Micro.checksum = 0;
	Usart_Trama_Micro.dato_1_byte = 0;
	Usart_Trama_Micro.dato_2_byte = 0;
	Usart_Trama_Micro.comando = 0;
	Usart_Trama_Micro.Count = 0;
}


void Usart_Reinicio_Trama(){
	
	// Reinicializar todos los campos del buffer
	Usart_Trama_Micro.trama_disponible = false;
	Usart_Trama_Micro.checksum = 0;
	Usart_Trama_Micro.dato_1_byte = 0;
	Usart_Trama_Micro.dato_2_byte = 0;
	Usart_Trama_Micro.comando = 0;
	Usart_Trama_Micro.Count = 0;
}

void Usart_Enviar_Ascii(uint8_t Puerto, const char *valor_enviar) {
	// Puntero al registro UART correspondiente
	USART_t *usart = (Puerto == USART_PC) ? &USARTC0 : &USARTC1;

	// Iterar sobre la cadena hasta encontrar el terminador nulo
	for (const char *ptr = valor_enviar; *ptr != '\0'; ptr++) {
		// Esperar hasta que el buffer de transmisión esté listo
		while (!(usart->STATUS & USART_DREIF_bm));
		usart->DATA = *ptr; // Enviar el carácter actual
	}
}


void Usart_Procesar_Datos(){
	uint8_t Dato_Rx = 0;
	static uint8_t Comprobar_Cheksum = 0;
	
	if(!Buffer_Esta_Vacio()){ //si hay datos en el buffer
		Buffer_Leer(&Dato_Rx);
		
		
		switch(Usart_Trama_Micro.Count){
			case 0:
			if(Dato_Rx == COMIENZO_TRAMA){
				Usart_Trama_Micro.Count++;
			Usart_Enviar_Ascii(0, "Detecto el Inicio de Trama\n\r");
			}
			break;
			
			case 1:
			Usart_Trama_Micro.tamano_trama = Dato_Rx;
			Comprobar_Cheksum += Usart_Trama_Micro.tamano_trama;
			Usart_Trama_Micro.Count++;
			
			lcd_gotorc(1,1);
			lcd_puts("T_Trama:");
			
			sprintf(letra, "%x", Usart_Trama_Micro.tamano_trama);
			lcd_gotorc(1,9);
			lcd_puts(letra);
			
			Usart_Enviar_Ascii(0, "Detecto el Tamano de trama\n\r");
			break;
			
			case 2:
			Usart_Trama_Micro.comando = Dato_Rx;
			Comprobar_Cheksum += Usart_Trama_Micro.comando;
			Usart_Trama_Micro.tamano_trama--;
			Usart_Trama_Micro.Count++;
			
			lcd_gotorc(2,1);
			lcd_puts("Comando:");
			
			sprintf(letra, "%c", Usart_Trama_Micro.comando);
			lcd_gotorc(2,9);
			lcd_puts(letra);
			
			Usart_Enviar_Ascii(0, "Detecto el Comando de trama\n\r");
			break;
			
			case 3:
			if(Usart_Trama_Micro.tamano_trama!= 0){
				if(Usart_Trama_Micro.tamano_trama == 2){
					Usart_Trama_Micro.dato_2_byte = Dato_Rx;
					Usart_Trama_Micro.tamano_trama--;
					Comprobar_Cheksum += Usart_Trama_Micro.dato_2_byte;
					Usart_Enviar_Ascii(0, "Detecto el Dato del byte 2\n\r");
				}
				else if(Usart_Trama_Micro.tamano_trama == 1){
					Usart_Trama_Micro.dato_1_byte = Dato_Rx;
					Usart_Trama_Micro.tamano_trama--;
					Comprobar_Cheksum += Usart_Trama_Micro.dato_1_byte;
					Usart_Trama_Micro.Count++;
					
					lcd_gotorc(3,1);
					lcd_puts("C_Byte:");
					
					sprintf(letra, "%x", Usart_Trama_Micro.dato_1_byte);
					lcd_gotorc(3,9);
					lcd_puts(letra);
			
					Usart_Enviar_Ascii(0, "Detecto el Dato del byte 1\n\r");
				}
				else{
					lcd_gotorc(4,1);
					lcd_puts("Error trama  ");//Informar que hubo un error en el tamano de la trama(bytes a enviar)
					Usart_Reinicio_Trama();		//quitar luego por que en usart_error_trama lo hacce
					Usart_Enviar_Ascii(0, "Error trama\n\r");
				}
				break;
			}
			
			case 4:
			Usart_Trama_Micro.checksum = Dato_Rx;
			
			lcd_gotorc(2,12);
			lcd_puts("ChK_L");
			
			sprintf(letra, "%x", Usart_Trama_Micro.checksum);
			lcd_gotorc(2,18);
			lcd_puts(letra);

			lcd_gotorc(3,12);
			lcd_puts("Chk_C");
			
			sprintf(letra, "%x", Comprobar_Cheksum);
			lcd_gotorc(3,18);
			lcd_puts(letra);
			
			if(Comprobar_Cheksum == Usart_Trama_Micro.checksum){
				lcd_gotorc(4,1);
				lcd_puts("Trama Correcta");
				Usart_Trama_Micro.trama_disponible = true;
				
				sprintf(letra, "%i", ++contador);
				lcd_gotorc(4,18);
				lcd_puts(letra);
				
			Usart_Enviar_Ascii(0, "Trama Correcta\n\r");
			}
			else{
				lcd_gotorc(4,1);
				lcd_puts("Error Cheksum");
				Usart_Enviar_Ascii(0, "Error cheksum\n\r");
				Usart_Reinicio_Trama();
			}
			Comprobar_Cheksum = 0;
			Usart_Trama_Micro.Count = 0;
			break;
		}
	}
}
