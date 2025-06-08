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

#define TAMMAX 1  // Tama�o m�ximo del buffer

typedef struct {
	uint8_t lista[TAMMAX];  // Arreglo para almacenar los elementos de la cola
	int8_t ini;           // �ndice del primer elemento en la cola
	int8_t fin;           // �ndice del �ltimo elemento en la cola
	int8_t count;         // N�mero actual de elementos en la cola
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

	Buffer_Add(&buffer, 10);  // Agregamos elementos usando la nueva funci�n Buffer_Add
	Buffer_Add(&buffer, 20);
	Buffer_Add(&buffer, 30);

	int valor;
	Buffer_Leer(&buffer, &valor);  // Leemos un elemento usando la nueva funci�n Buffer_Leer
	printf("Elemento le�do: %d\n", valor);

	Buffer_Leer(&buffer, &valor);  // Leemos otro elemento
	printf("Elemento le�do: %d\n", valor);

	return 0;
}

//Salida por cmd
Elemento le�do: 10
Elemento le�do: 20

*/