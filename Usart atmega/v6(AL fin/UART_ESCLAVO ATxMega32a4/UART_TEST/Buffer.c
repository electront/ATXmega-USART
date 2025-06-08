/*
 * Buffer.c
 *
 * Created: 24/1/2025 05:05:12
 *  Author: Aleman
 */ 

#include "ATXMega_lcd.h"
#include "Buffer.h"
#include "usart.h"
#include <avr/io.h>


// Función para inicializar el buffer
void Buffer_Inicializar() {
	Buffer_Micro.ini = 0;
	Buffer_Micro.fin = -1;
	Buffer_Micro.count = 0;
}

// Función para verificar si la cola está vacía
bool Buffer_Esta_Vacio() {
	return Buffer_Micro.count == 0;
}

// Función para verificar si la cola está llena
bool Buffer_Esta_Lleno(Buffer_s *buffer) {
	return Buffer_Micro.count == TAMMAX;
}

// Función para agregar un elemento al final de la cola (renombrada a Buffer_Add)
bool Buffer_Add(uint8_t valor) {
	Buffer_Micro.fin = (Buffer_Micro.fin + 1) % TAMMAX;  // Aseguramos que el índice se ajuste al tamaño del buffer
	Buffer_Micro.lista[Buffer_Micro.fin] = valor;
	Buffer_Micro.count++;
	return true;
}

// Función para eliminar un elemento del frente de la cola (renombrada a Buffer_Leer)
bool Buffer_Leer(uint8_t *valor) {
	*valor = Buffer_Micro.lista[Buffer_Micro.ini];
	Buffer_Micro.ini = (Buffer_Micro.ini + 1) % TAMMAX;  // Ajustamos el índice del inicio
	Buffer_Micro.count--;
	return true;
}


