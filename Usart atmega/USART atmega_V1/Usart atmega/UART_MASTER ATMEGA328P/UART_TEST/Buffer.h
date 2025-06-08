/*
 * Buffer.h
 *
 * Created: 24/1/2025 05:05:29
 *  Author: Aleman
 */ 


#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdbool.h>
#include <avr/io.h>

#define TAMMAX 100

typedef struct{
	int list[TAMMAX];
	int ini, fin;
}Buffer_s;


Buffer_s Buffer_Inicializar();
bool Buffer_Esta_Vacio(Buffer_s buffer);
bool Buffer_Esta_Lleno(Buffer_s buffer);
void Buffer_Escribir(Buffer_s *buffer, uint8_t elem);
uint8_t Buffer_Leer(Buffer_s *buffer);

#endif /* BUFFER_H_ */