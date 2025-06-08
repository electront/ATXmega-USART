/*
 * Buffer.c
 *
 * Created: 24/1/2025 05:05:12
 *  Author: Aleman
 */ 


#include "Buffer.h"
#include <avr/io.h>

/*
Buffer_s Buffer_Inicializar(){
	Buffer_s buffer;
	buffer.ini = buffer.fin = 0;

	return buffer;
}
*/

void Buffer_Inicializar(){
	Buffer_s.ini = 0;
	Buffer_s.fin = 0;
}


bool Buffer_Esta_Vacio(Buffer_s buffer){
	if(buffer.ini == buffer.fin){
		return true; //Esta vacia la cola.
	}
	else{
		return false; //No esta vacia la cola.
	}
}

bool Buffer_Esta_Lleno(Buffer_s buffer){
	//Hay dos pisibles casos por los que puede estar llena la cola.
	if(buffer.fin == TAMMAX - 1 && buffer.ini == 0){
		return true; //La cola esta llena
	}
	else if(buffer.fin == buffer.ini - 1){
		return true; //La cola esta llena.
	}
	else{
		return false; //La cola no esta llena.
	}
}

void Buffer_Escribir(Buffer_s *buffer, uint8_t elem){
	if(Buffer_Esta_Lleno(*buffer) == false){
		buffer->list[buffer->fin] = elem;
		if(buffer->fin == TAMMAX -1){
			buffer->fin = 0;
		}
		else{
			buffer->fin = buffer->fin + 1;
		}
	}
	else{
		//printf("La cola esta llena\n");
	}
}


uint8_t Buffer_Leer(Buffer_s *buffer){
	uint8_t elem;

	if(Buffer_Esta_Vacio(*buffer) == false){
		elem = buffer->list[buffer->ini];
		if(buffer->ini == TAMMAX - 1){
			buffer->ini = 0;
		}
		else{
			buffer->ini = buffer->ini +1;
		}

		return elem;
	}
	else{
		//printf("La cola esta vacia\n");
		return -1;
	}
}