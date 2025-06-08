/*
 * Buffer.c
 *
 * Created: 24/1/2025 05:05:12
 *  Author: Aleman
 */ 

#include "ATXMega_lcd.h"
#include "Buffer.h"
#include <avr/io.h>


// Función para inicializar el buffer
void Buffer_Inicializar(Buffer_s *buffer) {
	buffer->ini = 0;
	buffer->fin = 0;
	buffer->count = 0;
}

// Función para verificar si la cola está vacía
bool Buffer_Esta_Vacio(Buffer_s *buffer) {
	return buffer->count == 0;
}

// Función para verificar si la cola está llena
bool Buffer_Esta_Lleno(Buffer_s *buffer) {
	return buffer->count == TAMMAX;
}

// Función para agregar un elemento al final de la cola (renombrada a Buffer_Add)
bool Buffer_Add(Buffer_s *buffer, unsigned char valor) {
	if (Buffer_Esta_Lleno(buffer)) {
		Usart_Enviar_Ascii(0, "buffer Lleno\n\r");
		return false;
	}
	buffer->fin = (buffer->fin + 1) % TAMMAX;  // Aseguramos que el índice se ajuste al tamaño del buffer
	buffer->list[buffer->fin] = valor;
	buffer->count++;
	Usart_Enviar_Ascii(0, "add buffer\n\r");
	return true;
}

// Función para eliminar un elemento del frente de la cola (renombrada a Buffer_Leer)
bool Buffer_Leer(Buffer_s *buffer, unsigned char *valor) {
	if (Buffer_Esta_Vacio(buffer)) {
		//Usart_Enviar_Ascii(0, "Buffer vacio\n\r");
		return false;
	}
	*valor = buffer->list[buffer->ini];
	buffer->ini = (buffer->ini + 1) % TAMMAX;  // Ajustamos el índice del inicio
	buffer->count--;
	
	Usart_Enviar_Ascii(0, "leyo un buffer\n\r");
	return true;
}


