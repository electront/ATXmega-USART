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

Buffer_s Buffer_Micro;
Buffer_s Buffer_PC;


char letra[5];
int contador = 0;

ISR(USARTC0_RXC_vect){
	uint8_t data = USARTC0.DATA; // Leer dato recibido
	
	if(!Buffer_Esta_Lleno(&Buffer_PC)){ // Reviso si el buffer está lleno
		Buffer_Add(&Buffer_PC, data);	 // Escribir en el buffer
	}
}

ISR(USARTC1_RXC_vect){
	uint8_t data = USARTC1.DATA; // Leer dato recibido
	
	if(!Buffer_Esta_Lleno(&Buffer_Micro)){ // Reviso si el buffer está lleno
		Buffer_Add(&Buffer_Micro, data);	 // Escribir en el buffer
	}
}


void Usart_Inicializar(uint8_t puerto){
	if(puerto == USART_PC){
		PORTC.DIRSET = PIN3_bm; // TX
		PORTC.DIRCLR = PIN2_bm; // RX
		USARTC0.CTRLA = USART_RXCINTLVL_MED_gc; // Habilitando interrupción por recepción.
		USARTC0.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Habilitando recepción y transmisión.
		USARTC0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
		USARTC0.BAUDCTRLA = 0x03; // Baudrate 9600.
		USARTC0.BAUDCTRLB = 0x96;
		Usart_Inicializar_Buffer(BUFFER_PC); // Inicializar Buffer y Trama.
		}else if(puerto == USART_MICRO) {
		PORTC.DIRSET = PIN7_bm; // TX
		PORTC.DIRCLR = PIN6_bm; // RX
		USARTC1.CTRLA = USART_RXCINTLVL_MED_gc; // Habilitando interrupción por recepción.
		USARTC1.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Habilitando recepción y transmisión.
		USARTC1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
		USARTC1.BAUDCTRLA = 0x03; // Baudrate 9600.
		USARTC1.BAUDCTRLB = 0x96;
		Usart_Inicializar_Buffer(BUFFER_MICRO); // Inicializar Buffer y Trama.
	}
}


void Usart_Inicializar_Buffer(uint8_t buffer) {
	
	//Uso el if para Inicizaliar el buffer que voy a usar
	//if(buffer == BUFFER_PC){
		//Buffer_Inicializar(&Buffer_PC);//}
	//else{
		Buffer_Inicializar(&Buffer_Micro);//}
	
	// Puntero al buffer correspondiente
	//struct Usart_Trama_Struct *Usart_Trama = (buffer == BUFFER_PC) ? &Usart_Trama_PC : &Usart_Trama_Micro;

	// Inicializar todos los campos del buffer
	Usart_Trama_Micro.deteccion_comienzo_trama = false;
	Usart_Trama_Micro.deteccion_tamano_trama = false;
	Usart_Trama_Micro.deteccion_comando_trama = false;
	Usart_Trama_Micro.deteccion_checksum = false;
	Usart_Trama_Micro.trama_disponible = false;
	Usart_Trama_Micro.checksum = 0;
	Usart_Trama_Micro.dato_1_byte = 0;
	Usart_Trama_Micro.dato_2_byte = 0;
	Usart_Trama_Micro.comando = 0;
	Usart_Trama_Micro.Count = 0;
}


void Usart_Reinicio_Trama(uint8_t buffer){
	// Puntero al buffer correspondiente
	struct Usart_Trama_Struct *Usart_Trama = (buffer == BUFFER_PC) ? &Usart_Trama_PC : &Usart_Trama_Micro;

	// Reinicializar todos los campos del buffer
	Usart_Trama_Micro.deteccion_comienzo_trama = false;
	Usart_Trama_Micro.deteccion_tamano_trama = false;
	Usart_Trama_Micro.deteccion_comando_trama = false;
	Usart_Trama_Micro.deteccion_checksum = false;
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

void Usart_Enviar_Hex(uint8_t Puerto, unsigned char valor_enviar) {
	// Puntero al registro UART correspondiente
	USART_t *usart = (Puerto == USART_PC) ? &USARTC0 : &USARTC1;

	// Esperar hasta que el buffer de transmisión esté listo
	while (!(usart->STATUS & USART_DREIF_bm));

	// Enviar el byte
	usart->DATA = valor_enviar;
}


void Usart_Error_Trama(uint8_t Puerto, uint8_t Buffer){
	//hallando checksum
	uint8_t checksum = 0;
	
	checksum += 0X02; //Tamano trama.
	checksum += ERROR_TRAMA;
	checksum += 0x00; //Para competar
	
	Usart_Enviar_Hex(Puerto, COMIENZO_TRAMA);
	Usart_Enviar_Hex(Puerto, 0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Hex(Puerto, ERROR_TRAMA); //Envio del comando error.
	Usart_Enviar_Hex(Puerto, 0x00); // Envio 0x00 para rellenar.
	Usart_Enviar_Hex(Puerto, checksum);	//checksum

	Usart_Reinicio_Trama(Buffer);
}



void Usart_Procesar_Datos(){
	uint8_t Dato_Rx = 0;
	static uint8_t Comprobar_Cheksum = 0;
	
	if(!Buffer_Esta_Vacio(&Buffer_Micro)){ //si hay datos en el buffer
		Buffer_Leer(&Buffer_Micro, &Dato_Rx);
		
		
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
			
			sprintf(letra, "%x", Usart_Trama_Micro.comando);
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
					Usart_Reinicio_Trama(BUFFER_MICRO);		//quitar luego por que en usart_error_trama lo hacce
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
				Usart_Reinicio_Trama(BUFFER_MICRO);
			}
			Comprobar_Cheksum = 0;
			Usart_Trama_Micro.Count = 0;
			break;
		}
	}
}



/*

void Usart_Enviar_Fin_Datos(){
	uint8_t checksum = 0;
	
	checksum ^= 0x01;
	checksum ^= S_FIN_DATOS;
	
	uint8_t trama_enviar[4] = {COMIENZO_TRAMA, 0x01, S_FIN_DATOS, checksum};
	
	for(uint8_t i = 0; i < 5; i++){
		if(Buffer_Esta_Lleno(&Buffer_Entrada) == false){
			Buffer_Add(&Buffer_Entrada, trama_enviar[i]);
		}
	}
	
	
	 Usart_Enviar_Hex(COMIENZO_TRAMA);
	 Usart_Enviar_Hex(0x01);
	 Usart_Enviar_Hex(S_FIN_DATOS);
	 Usart_Enviar_Hex(checksum);
	
	
	
	if(Usart_Esta_Enviando_Datos() == false){
		uint8_t valor_enviar = 0;
		Buffer_Leer(&Buffer_Entrada, &valor_enviar);
		Usart_Enviar_Hex(valor_enviar);
	}
}

bool Usart_Esta_Enviando_Datos(void){
	if (!(UCSR0A & (1 << UDRE0))) { // Si el buffer de transmisión NO está vacío, se están enviando datos.
	return true; } // Se está enviando datos.
	else {
	return false; }// No se está enviando datos.
}

void Usart_Enviar_Comando(Command comando){ //Enviar comando.
	uint8_t checksum = 0;
	
	checksum ^= 0x02;
	checksum ^= ENVIO_COMANDO;
	checksum ^= comando;
	
	Usart_Enviar_Hex(COMIENZO_TRAMA);
	Usart_Enviar_Hex(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Hex(ENVIO_COMANDO);
	Usart_Enviar_Hex(comando);
	Usart_Enviar_Hex(checksum);	//checksum
}

void Usart_Enviar_Sensado(Command comando, uint16_t valor_enviar){ //Enviar comando y valor usando el protocolo.
	uint8_t checksum = 0;
	
	Usart_Enviar_Hex(COMIENZO_TRAMA);
	if(valor_enviar >= 256){
		
		checksum ^= 0x03;
		checksum ^= comando;
		checksum ^= valor_enviar >> 8;
		checksum ^= valor_enviar;

		Usart_Enviar_Hex(0x03); //Significa que se va a enviar 3 byte, (comando, 2 byte de datos);
		Usart_Enviar_Hex(comando);
		Usart_Enviar_Hex(valor_enviar >> 8); //Envio el byte mas significativo.
		Usart_Enviar_Hex(valor_enviar); //Envio el byte menos significativo.
		Usart_Enviar_Hex(checksum); //checksum
		
	}
	else{
		
		checksum ^= 0x02;
		checksum ^= comando;
		checksum ^= valor_enviar;
		
		
		Usart_Enviar_Hex(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
		Usart_Enviar_Hex(comando);
		Usart_Enviar_Hex(valor_enviar); //Envio el byte.
		Usart_Enviar_Hex(checksum);	//checksum
		
	}
}


*/