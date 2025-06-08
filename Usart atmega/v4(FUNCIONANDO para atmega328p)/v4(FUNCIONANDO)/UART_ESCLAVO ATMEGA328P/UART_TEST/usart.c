/*
 * Puerto_Serie_AtxMega.c
 *
 * Created: 27/01/2022 12:45:47
 *  Author: usuario
 */ 


#include "usart.h"
#include "lcd.h"


//Los declaro en el '.c' para que no puedan ser accedidos por nada fuera de
//este archivo.

char letra[5];

ISR(USART_RX_vect){
	uint8_t data = UDR0; // Leer dato recibido
	if(!Buffer_Esta_Lleno(&Buffer_Entrada)){ // Reviso si el buffer está lleno
		Buffer_Add(&Buffer_Entrada, data);	 // Escribir en el buffer
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
	
	Buffer_Inicializar(&Buffer_Entrada);

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
	Usart_Trama.Count = 0;
	
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
	Usart_Trama.Count = 0;
}


void Usart_Guardar_Valor_Buffer(){
	static uint16_t comprobar_checksum = 0;
	
	uint8_t valor_leido = 0;
	
	if(Buffer_Esta_Vacio(&Buffer_Entrada)){
		return;
	}
	else{
	Buffer_Leer(&Buffer_Entrada, &valor_leido);
	
	if(!Usart_Trama.deteccion_comienzo_trama){
		if(valor_leido == COMIENZO_TRAMA){
			Usart_Trama.deteccion_comienzo_trama = true; // Indico que ya recibi el inicio de la trama.
		}
	}
	else{ // Si ya se recibio el inicio de trama continuar con la recepcion.
		if(Usart_Trama.deteccion_tamano_trama == false){
			Usart_Trama.tamano_trama = valor_leido; //Guardado el tamano de la trama.
			Usart_Trama.deteccion_tamano_trama = true; // Indico que ya recibi tamano de la trama.
			comprobar_checksum += Usart_Trama.tamano_trama;
			
					lcd_gotorc(1,1);
					lcd_puts("T_Trama");
						
					sprintf(letra, "%d", Usart_Trama.tamano_trama);
					lcd_gotorc(1,9);
					lcd_puts(letra);			
		}
		else if(Usart_Trama.deteccion_comando_trama == false){
			Usart_Trama.comando = valor_leido;
			Usart_Trama.tamano_trama--; //Resto '1' pq ya quite el comando de la trama.
			Usart_Trama.deteccion_comando_trama = true;
			comprobar_checksum += Usart_Trama.comando;
			
					lcd_gotorc(2,1);
					lcd_puts("Comando");
						
					sprintf(letra, "%d", Usart_Trama.comando);
					lcd_gotorc(2,9);
					lcd_puts(letra);
			
			
		}
		else if(Usart_Trama.tamano_trama != 0){
			if(Usart_Trama.tamano_trama == 2){
				Usart_Trama.dato_2_byte = valor_leido;
				Usart_Trama.tamano_trama--; //Resto '1' pq ya quite el byte_2 de la trama.
				comprobar_checksum += Usart_Trama.dato_2_byte;
			}
			else if(Usart_Trama.tamano_trama == 1){
				Usart_Trama.dato_1_byte = valor_leido;
				Usart_Trama.tamano_trama--; //Resto '1' pq ya quite el byte_1 de la trama.
				comprobar_checksum += Usart_Trama.dato_1_byte;
				
					lcd_gotorc(3,1);
					lcd_puts("datByte");
							
					sprintf(letra, "%d", Usart_Trama.dato_1_byte);
					lcd_gotorc(3,9);
					lcd_puts(letra);
					

							
			}
			else{
				lcd_gotorc(4,1);
				lcd_puts("Error trama  ");//Informar que hubo un error en el tamano de la trama(bytes a enviar)
				//Usart_Error_Trama();// Informa que hubo un error en la trama y reinicia el procesamiento con el siguiente trama a procesar.
				Usart_Reinicio_Trama();		//quitar luego por que en usart_error_trama lo hacce
			}
		}
		else{
			Usart_Trama.checksum = valor_leido;
		
			lcd_gotorc(2,11);
			lcd_puts("ChK_L");
							
			sprintf(letra, "%d", Usart_Trama.checksum);
			lcd_gotorc(2,18);
			lcd_puts(letra);

			lcd_gotorc(3,11);
			lcd_puts("Chk_C");
							
			sprintf(letra, "%d", comprobar_checksum);
			lcd_gotorc(3,18);
			lcd_puts(letra);						
							
			if(Usart_Trama.checksum == comprobar_checksum){
				lcd_gotorc(4,1);
				lcd_puts("Trama Correcta");
				//Serie_enviar_ascii("sip");
				Usart_Trama.trama_disponible = true;

			}
			else{
				lcd_gotorc(4,1);
				lcd_puts("Error Cheksum ");
				Usart_Reinicio_Trama();
				//Serie_enviar_ascii(USART_PC, "nop\n");
				//Usart_Error_Trama();
				
				}
			comprobar_checksum = 0;
			}
		}
	}
}


void Usart_Procesar_Datos(){
	uint8_t Dato_Rx = 0;
	static uint8_t Comprobar_Cheksum = 0;
	
	if(!Buffer_Esta_Vacio(&Buffer_Entrada)){ //si hay datos en el buffer
		Buffer_Leer(&Buffer_Entrada, &Dato_Rx);
		
		switch(Usart_Trama.Count){
			case 0:
			if(Dato_Rx == COMIENZO_TRAMA){
				Usart_Trama.Count++;
			}
			break;
			
			case 1:
			Usart_Trama.tamano_trama = Dato_Rx;
			Comprobar_Cheksum += Usart_Trama.tamano_trama;
			Usart_Trama.Count++;
			
			lcd_gotorc(1,1);
			lcd_puts("T_Trama");
								
			sprintf(letra, "%x", Usart_Trama.tamano_trama);
			lcd_gotorc(1,9);
			lcd_puts(letra);
								
			break;
			
			case 2:
			Usart_Trama.comando = Dato_Rx;
			Comprobar_Cheksum += Usart_Trama.comando;
			Usart_Trama.tamano_trama--;
			Usart_Trama.Count++;
			
			lcd_gotorc(2,1);
			lcd_puts("Comando");
					
			sprintf(letra, "%x", Usart_Trama.comando);
			lcd_gotorc(2,9);
			lcd_puts(letra);
			break;
			
			case 3:
			if(Usart_Trama.tamano_trama!= 0){
				if(Usart_Trama.tamano_trama == 2){
					Usart_Trama.dato_2_byte = Dato_Rx;
					Usart_Trama.tamano_trama--;
					Comprobar_Cheksum += Usart_Trama.dato_2_byte;
				}
				else if(Usart_Trama.tamano_trama == 1){
					Usart_Trama.dato_1_byte = Dato_Rx;
					Usart_Trama.tamano_trama--;
					Comprobar_Cheksum += Usart_Trama.dato_1_byte;
					Usart_Trama.Count++;
					
					lcd_gotorc(3,1);
					lcd_puts("datByte");
			
					sprintf(letra, "%x", Usart_Trama.dato_1_byte);
					lcd_gotorc(3,9);
					lcd_puts(letra);
				}
				else{
					lcd_gotorc(4,1);
					lcd_puts("Error trama  ");//Informar que hubo un error en el tamano de la trama(bytes a enviar)
					Usart_Reinicio_Trama();		//quitar luego por que en usart_error_trama lo hacce
				}
			break;
			}
			
			case 4:
			Usart_Trama.checksum = Dato_Rx;
			
			lcd_gotorc(2,11);
			lcd_puts("ChK_L");
							
			sprintf(letra, "%x", Usart_Trama.checksum);
			lcd_gotorc(2,18);
			lcd_puts(letra);

			lcd_gotorc(3,11);
			lcd_puts("Chk_C");
							
			sprintf(letra, "%x", Comprobar_Cheksum);
			lcd_gotorc(3,18);
			lcd_puts(letra);	
			
			if(Comprobar_Cheksum == Usart_Trama.checksum){
				lcd_gotorc(4,1);
				lcd_puts("Trama Correcta");
				Usart_Trama.trama_disponible = true;
			}
			else{
				lcd_gotorc(4,1);
				lcd_puts("Error Cheksum   ");
				Usart_Reinicio_Trama();
			}
			Comprobar_Cheksum = 0;
			Usart_Trama.Count = 0;
			break;
		}
	}
}

void Usart_Enviar_Caracter(unsigned char caracter){
	while(!(UCSR0A&(1<<5)));				// mientras el registro UDR0 esté lleno espera
	UDR0 = caracter;						//cuando el el registro UDR0 está vacio se envia el caracter
}

void Usart_Enviar_Ascii(char* cadena){			//cadena de caracteres de tipo char
	while(*cadena !=0x00)
	{
		Usart_Enviar_Caracter(*cadena);				//transmite los caracteres de cadena
		cadena++;							//incrementa la ubicación de los caracteres en cadena
		//para enviar el siguiente caracter de cadena
	}
}
