/*
 * Puerto_Serie_AtxMega.c
 *
 * Created: 27/01/2022 12:45:47
 *  Author: usuario
 */ 


#include "usart.h"

void Usart_init(){
	
	__USART_OUTPUT_TX(); // Habilitando como salida TX.
	__USART_INPUT_RX(); //Habilitando como entrada RX.
	__USART_ENABLE_TX_RX();
	__USART_ENABLE_IRQ_RX(); //Habilito interrupcion por Recepcion, habilito transmicion y recepcion.
    __USART_MODE_AS_IMP_1STOP();//Habilitando modo Asyncronico, Paridad impar, 1 bit stop.
	__USART_BAUD_9600();//Baudrate 9600 velocidad simple.
	
}

void Usart_Enviar_Dato(unsigned char Dato){
	while(!(__USART_WAIT()));
	__USART_BUFFER = Dato;
}
