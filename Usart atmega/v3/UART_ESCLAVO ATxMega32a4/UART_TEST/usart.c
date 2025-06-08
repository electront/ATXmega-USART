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

ISR(USARTC0_RXC_vect){
	unsigned char data = USARTC0.DATA; // Leer dato recibido
	
	if(!Buffer_Esta_Lleno(&Buffer_PC)){ // Reviso si el buffer está lleno
		Buffer_Add(&Buffer_PC, data);	 // Escribir en el buffer
	}
}

ISR(USARTC1_RXC_vect){
	unsigned char data = USARTC1.DATA; // Leer dato recibido
	
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
		USARTC1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_ODD_gc | USART_CHSIZE_8BIT_gc;
		USARTC1.BAUDCTRLA = 0x03; // Baudrate 9600.
		USARTC1.BAUDCTRLB = 0x96;
		Usart_Inicializar_Buffer(BUFFER_MICRO); // Inicializar Buffer y Trama.
	}
}


void Usart_Inicializar_Buffer(uint8_t buffer) {
	
	//Uso el if para Inicizaliar el buffer que voy a usar
	//if(buffer == BUFFER_PC){
		Buffer_Inicializar(&Buffer_PC);//}
	//else{
		Buffer_Inicializar(&Buffer_Micro);//}
	
	// Puntero al buffer correspondiente
	struct Usart_Trama_Struct *Usart_Trama = (buffer == BUFFER_PC) ? &Usart_Trama_PC : &Usart_Trama_Micro;

	// Inicializar todos los campos del buffer
	Usart_Trama->deteccion_comienzo_trama = false;
	Usart_Trama->deteccion_tamano_trama = false;
	Usart_Trama->deteccion_comando_trama = false;
	Usart_Trama->deteccion_checksum = false;
	Usart_Trama->trama_disponible = false;
	Usart_Trama->checksum = 0;
	Usart_Trama->dato_1_byte = 0;
	Usart_Trama->dato_2_byte = 0;
	Usart_Trama->comando = 0;
}


void Usart_Reinicio_Trama(uint8_t buffer){
	// Puntero al buffer correspondiente
	struct Usart_Trama_Struct *Usart_Trama = (buffer == BUFFER_PC) ? &Usart_Trama_PC : &Usart_Trama_Micro;

	// Reinicializar todos los campos del buffer
	Usart_Trama->deteccion_comienzo_trama = false;
	Usart_Trama->deteccion_tamano_trama = false;
	Usart_Trama->deteccion_comando_trama = false;
	Usart_Trama->deteccion_checksum = false;
	Usart_Trama->trama_disponible = false;
	Usart_Trama->checksum = 0;
	Usart_Trama->dato_1_byte = 0;
	Usart_Trama->dato_2_byte = 0;
	Usart_Trama->comando = 0;
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



void Usart_Guardar_Valor_Buffer(unsigned char Buffer){
	
	static unsigned char comprobar_checksum = 0;
	unsigned char valor_leido = 0;
	
	// Puntero a la Trama correspondiente
	struct Usart_Trama_Struct *Usart_Trama = &Usart_Trama_Micro;// = (Buffer == BUFFER_PC) ? &Usart_Trama_PC : &Usart_Trama_Micro;


	//if(Buffer == BUFFER_PC){
		//Buffer_Leer(&Buffer_PC, &valor_leido);
		//}
//	else if(Buffer == BUFFER_MICRO){
		Buffer_Leer(&Buffer_Micro, &valor_leido);
	//	}	
					
	if(!Usart_Trama->deteccion_comienzo_trama){
		if(valor_leido == COMIENZO_TRAMA){
			Usart_Enviar_Ascii(USART_PC, "Comienzo trama\n\r");
			Usart_Trama->deteccion_comienzo_trama = true; // Indico que ya recibi el inicio de la trama.
		}
	}
	else{ // Si ya se recibio el inicio de trama continuar con la recepcion.
		if(Usart_Trama->deteccion_tamano_trama == false){
			Usart_Trama->tamano_trama = valor_leido; //Guardado el tamano de la trama.
			Usart_Trama->deteccion_tamano_trama = true; // Indico que ya recibi tamano de la trama.
			comprobar_checksum ^= Usart_Trama->tamano_trama;	
			Usart_Enviar_Ascii(USART_PC, "Detecto el tamano de trama\n\r");		
		}
		else if(Usart_Trama->deteccion_comando_trama == false){
			Usart_Trama->comando = valor_leido;
			Usart_Trama->tamano_trama--; //Resto '1' pq ya quite el comando de la trama.
			Usart_Trama->deteccion_comando_trama = true;
			comprobar_checksum ^= Usart_Trama->comando;
			Usart_Enviar_Ascii(USART_PC, "Detecto el Comando\n\r");	
		}
		else if(Usart_Trama->tamano_trama != 0){
			if(Usart_Trama->tamano_trama == 2){
				Usart_Trama->dato_2_byte = valor_leido;
				Usart_Trama->tamano_trama--; //Resto '1' pq ya quite el byte_2 de la trama.
				comprobar_checksum ^= Usart_Trama->dato_2_byte;
				Usart_Enviar_Ascii(USART_PC, "Detecto el byte2\n\r");	
			}
			else if(Usart_Trama->tamano_trama == 1){
				Usart_Trama->dato_1_byte = valor_leido;
				Usart_Trama->tamano_trama--; //Resto '1' pq ya quite el byte_1 de la trama.
				comprobar_checksum ^= Usart_Trama->dato_1_byte;	
				Usart_Enviar_Ascii(USART_PC, "Detecto el byte1\n\r");		
			}
			else{
				//Usart_Error_Trama();// Informa que hubo un error en la trama y reinicia el procesamiento con el siguiente trama a procesar.
				Usart_Reinicio_Trama(Buffer);		//quitar luego por que en usart_error_trama lo hacce
				Usart_Enviar_Ascii(USART_PC, "Error de Trama\n\r");
			}
		}
		else{
			Usart_Trama->checksum = valor_leido;					
							
			if(Usart_Trama->checksum == comprobar_checksum){
				Usart_Enviar_Ascii(USART_PC, "Trama completada\n\r");
				Usart_Trama->trama_disponible = true;
			}
			else{
				Usart_Enviar_Ascii(USART_PC, "Error de Cheksum\n\r");
				Usart_Reinicio_Trama(Buffer);
				//Serie_enviar_ascii(USART_PC, "nop\n");
				//Usart_Error_Trama();
				
			}
			comprobar_checksum = 0;
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