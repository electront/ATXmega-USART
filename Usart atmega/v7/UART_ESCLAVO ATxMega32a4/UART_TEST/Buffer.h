/*
 * Buffer.h
 *
 * Created: 24/1/2025 05:05:29
 *  Author: Aleman
 */ 


#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>
#include <stdbool.h>

#define TAMMAX 1  // Tamaño máximo del buffer

typedef struct {
	uint8_t lista[TAMMAX];  // Arreglo para almacenar los elementos de la cola
	int8_t ini;           // Índice del primer elemento en la cola
	int8_t fin;           // Índice del último elemento en la cola
	int8_t count;         // Número actual de elementos en la cola
} Buffer_s;

Buffer_s Buffer_Micro;

void Buffer_Inicializar();
bool Buffer_Esta_Vacio();
bool Buffer_Esta_Lleno();
bool Buffer_Add(uint8_t valor);
bool Buffer_Leer(uint8_t *valor);

#endif /* BUFFER_H_ */


/*EJEMPLO DE COMO USARLA


int main() {
	Buffer_s buffer;	CREO UNA VARIABLE TIPO BUFFER
	Buffer_Inicializar(&buffer);  // Inicializamos el buffer

	Buffer_Add(&buffer, 10);  // Agregamos elementos usando la nueva función Buffer_Add
	Buffer_Add(&buffer, 20);
	Buffer_Add(&buffer, 30);

	int valor;
	Buffer_Leer(&buffer, &valor);  // Leemos un elemento usando la nueva función Buffer_Leer
	printf("Elemento leído: %d\n", valor);

	Buffer_Leer(&buffer, &valor);  // Leemos otro elemento
	printf("Elemento leído: %d\n", valor);

	return 0;
}

//Salida por cmd
Elemento leído: 10
Elemento leído: 20

*/