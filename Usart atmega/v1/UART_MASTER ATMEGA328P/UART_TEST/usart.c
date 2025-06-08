/*
 * Puerto_Serie_AtxMega.c
 *
 * Created: 27/01/2022 12:45:47
 *  Author: usuario
 */ 


#include "usart.h"

//Los declaro en el '.c' para que no puedan ser accedidos por nada fuera de
//este archivo.

Buffer_s Buffer_Entrada;


ISR(USART_RX_vect){
	uint8_t data = UDR0; // Leer dato recibido
	if(!Buffer_Esta_Lleno(Buffer_Entrada)){ // Reviso si el buffer está lleno
		Buffer_Escribir(&Buffer_Entrada, data); // Escribir en el buffer
	}
}

void Usart_init(){
	
	__USART_OUTPUT_TX(); // Habilitando como salida TX.
	__USART_INPUT_RX(); //Habilitando como entrada RX.
	__USART_ENABLE_TX_RX();
	__USART_ENABLE_IRQ_RX(); //Habilito interrupcion por Recepcion, habilito transmicion y recepcion.
    __USART_MODE_AS_IMP_1STOP();//Habilitando modo Asyncronico, Paridad impar, 1 bit stop.
	__USART_BAUD_9600();//Baudrate 9600 velocidad simple.
	
	Usart_Inicializar_Buffer();
}

void Usart_Inicializar_Buffer(){
	Buffer_Entrada = Buffer_Inicializar();

	// Inicializar todos los campos del buffer
	Usart_Trama.deteccion_comienzo_trama = false;
	Usart_Trama.deteccion_tamano_trama = false;
	Usart_Trama.deteccion_comando_trama = false;
	Usart_Trama.deteccion_checksum = false;
	Usart_Trama.trama_disponible = false;
	Usart_Trama.checksum = 0;
	Usart_Trama.dato_1_byte = 0;
	Usart_Trama.dato_2_byte = 0;
	Usart_Trama.comando = 0;
}

void Usart_Reinicio_Trama(){

	// Reinicializar todos los campos del buffer
	Usart_Trama.deteccion_comienzo_trama = false;
	Usart_Trama.deteccion_tamano_trama = false;
	Usart_Trama.deteccion_comando_trama = false;
	Usart_Trama.deteccion_checksum = false;
	Usart_Trama.trama_disponible = false;
	Usart_Trama.checksum = 0;
	Usart_Trama.dato_1_byte = 0;
	Usart_Trama.dato_2_byte = 0;
	Usart_Trama.comando = 0;
}

void Usart_Enviar_Hex(uint8_t valor_enviar){
	while(!(__USART_WAIT()));
	__USART_BUFFER = valor_enviar;
}

void Usart_Error_Trama(){
	//hallando checksum
	uint8_t checksum = 0;
	
	checksum ^= 0X02; //Tamano trama.
	checksum ^= ERROR_TRAMA;
	checksum ^= 0x00; //Para competar
	
	Usart_Enviar_Hex(COMIENZO_TRAMA);
	Usart_Enviar_Hex(0x02); //Significa que se va a enviar 2 byte, (comando, 1 byte de datos);
	Usart_Enviar_Hex(ERROR_TRAMA); //Envio del comando error.
	Usart_Enviar_Hex(0x00); // Envio 0x00 para rellenar.
	Usart_Enviar_Hex(checksum);	//checksum

	Usart_Reinicio_Trama();
}

void Usart_Guardar_Valor_Buffer(){
	static uint8_t comprobar_checksum = 0;
	uint8_t valor_leido = Buffer_Leer(&Buffer_Entrada);
	
	if(!Usart_Trama.deteccion_comienzo_trama){
		if( valor_leido == COMIENZO_TRAMA){
			Usart_Trama.deteccion_comienzo_trama = true; // Indico que ya recibi el inicio de la trama.
		}
	}
	else{ // Si ya se recibio el inicio de trama continuar con la recepcion.
		if(Usart_Trama.deteccion_tamano_trama == false){
			Usart_Trama.tamano_trama = valor_leido; //Guardado el tamano de la trama.
			Usart_Trama.deteccion_tamano_trama = true; // Indico que ya recibi tamano de la trama.
			comprobar_checksum ^= Usart_Trama.tamano_trama;
		}
		else if(Usart_Trama.deteccion_comando_trama == false){
			Usart_Trama.comando = valor_leido;
			Usart_Trama.tamano_trama--; //Resto '1' pq ya quite el comando de la trama.
			Usart_Trama.deteccion_comando_trama = true;
			comprobar_checksum ^= Usart_Trama.comando;
		}
		else if(Usart_Trama.tamano_trama != 0){
			if(Usart_Trama.tamano_trama == 2){
				Usart_Trama.dato_2_byte = valor_leido;
				Usart_Trama.tamano_trama--; //Resto '1' pq ya quite el byte_2 de la trama.
				comprobar_checksum ^= Usart_Trama.dato_2_byte;
			}
			else if(Usart_Trama.tamano_trama == 1){
				Usart_Trama.dato_1_byte = valor_leido;
				Usart_Trama.tamano_trama--; //Resto '1' pq ya quite el byte_2 de la trama.
				comprobar_checksum ^= Usart_Trama.dato_1_byte;
			}
			else{
				//Informar que hubo un error en el tamano de la trama(bytes a enviar)
				Usart_Error_Trama();// Informa que hubo un error en la trama y reinicia el procesamiento con el siguiente trama a procesar.
			}
		}
		else{
			Usart_Trama.checksum = valor_leido;
			if(Usart_Trama.checksum == comprobar_checksum){
				//Serie_enviar_ascii("sip");
				Usart_Trama.trama_disponible = true;
			}
			else{
				//Serie_enviar_ascii(USART_PC, "nop\n");
				Usart_Error_Trama();
			}
			comprobar_checksum = 0;
		}
	}
}

void Usart_Enviar_Fin_Datos(){
	uint8_t checksum = 0;
	
	checksum ^= 0x01;
	checksum ^= S_FIN_DATOS;
	
	uint8_t trama_enviar[4] = {COMIENZO_TRAMA, 0x01, S_FIN_DATOS, checksum};
	
	for(uint8_t i = 0; i < 5; i++){
		if(Buffer_Esta_Lleno(Buffer_Entrada) == false){
			Buffer_Escribir(&Buffer_Entrada, trama_enviar[i]);
		}
	}
	
	
	 Usart_Enviar_Hex(COMIENZO_TRAMA);
	 Usart_Enviar_Hex(0x01);
	 Usart_Enviar_Hex(S_FIN_DATOS);
	 Usart_Enviar_Hex(checksum);
	
	
	
	//if(Serie_Esta_Enviando_Datos() == false){
	//	uint8_t valor_enviar = Buffer_Leer(&Buffer_Entrada);
	//	Usart_Enviar_hex(valor_enviar);
	//}
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
