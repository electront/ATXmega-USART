/*
 * Puerto_Serie_AtxMega.c
 *
 * Created: 27/01/2022 12:45:47
 *  Author: usuario
 */ 


#include "usart.h"


void Usart_Enviar_Dato(uint8_t Dato){
	while(!(USARTC1.STATUS & USART_DREIF_bm));
	USARTC1.DATA = Dato;
}


void Usart_Inicializar(){
	PORTC.DIRSET = PIN7_bm; // TX
	PORTC.DIRCLR = PIN6_bm; // RX
	USARTC1.CTRLA = USART_RXCINTLVL_MED_gc; // Habilitando interrupción por recepción.
	USARTC1.CTRLB = USART_RXEN_bm | USART_TXEN_bm; // Habilitando recepción y transmisión.
	USARTC1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
	USARTC1.BAUDCTRLA = 0x03; // Baudrate 9600.
	USARTC1.BAUDCTRLB = 0x96;
}