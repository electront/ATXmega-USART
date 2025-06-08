
/******************************************************************************
 * FileName:   lcd.c
 * Purpose:    Librer�a de funciones para controlar un display LCD con chip
 *             Hitachi HD44780 o compatible. La interface es de 4 bits.
 * Processor:  ATmel AVR
 * Compiler:   IAR-C y AVR-GCC (WinAVR)
 * Author:     Shawn Johnson. http://www.cursomicros.com.
 *
 * Copyright (C) 2008 - 2012 Shawn Johnson. All rights reserved.
 *
 * License:    Se permiten el uso y la redistribuci�n de este c�digo con 
 *             modificaciones o sin ellas, siempre que se mantengan esta 
 *             licencia y las notas de autor y copyright de arriba.
 *****************************************************************************/

#include "ATXMega_lcd.h"

//****************************************************************************
// Ejecuta la inicializaci�n software completa del LCD. La configuraci�n es
// de: interface de 4 bits, despliegue de 2 l�neas y caracteres de 5x7 puntos.
//****************************************************************************
void lcd_init(void)
{
	/* Configurar las direcciones de los pines de interface del LCD */
    lcd_DATAddr |= 0xF0;   //DDRC  datos = ent  dir = sal
    lcd_CTRLddr |= (1<<lcd_E)|(1<<lcd_RW)|(1<<lcd_RS);   //DDRC dir de 1,2,3 = 1

    /* Secuencia de inicializaci�n del LCD en modo de 4 bits*/
   lcd_CTRLout &= ~((1<<lcd_E)|(1<<lcd_RW)|(1<<lcd_RS));   //en PORTC 1,2,3 = 0  

    ldelay_ms(45);         // > 40 ms
	
    lcd_nibble(0x30);      // Function Set: 8-bit
	//PORTC.OUT |= (1<<PIN0_bp);
    ldelay_ms(5);          // > 4.1 ms
	//PORTC.OUT &=~ (1<<PIN0_bp);
    lcd_nibble(0x30);      // Function Set: 8-bit
    ldelay_ms(1);          // > 100 �s
    lcd_nibble(0x30);      // Function Set: 8-bit
    ldelay_ms(1);          // > 40 �s
	
    lcd_nibble(0x20);      // Function Set: 4-bit
    ldelay_ms(3);          // > 40 �s
    lcd_nibble(0x20);      // Function Set: 4-bit, 2lines, 4�7font
    lcd_nibble(0x80);      //
    lcd_write(0x0C, 0); // Display ON/OFF Control: Display on, Cursor off, Blink off
    lcd_write(0x01, 0);    // Clear Display
    lcd_write(0x06, 0);    // Entry Mode Set
    lcd_clear();
}

//****************************************************************************
// Escribe una instrucci�n en el LCD:
// Si RS = 0 la instrucci�n es de comando (Function Set, Entry Mode set, etc).
// Si RS = 1 la instrucci�n es de dato y va a la DDRAM o CGRAM.
//****************************************************************************
void lcd_write(char inst, char RS)
{
    while(lcd_read(0)&0x80);         // Esperar mientras LCD est� ocupado
    if(RS)
        lcd_CTRLout |= (1<<lcd_RS);  // Para escribir en DDRAM o CGRAM
    else
        lcd_CTRLout &= ~(1<<lcd_RS); // Para escribir en Registro de Comandos
    _delay_us(12);                     // Permite actualizar el Puntero de RAM
    lcd_nibble(inst);                // Enviar nibble alto
    lcd_nibble(inst<<4);             // Enviar nibble bajo
}


//****************************************************************************
// Env�a el nibble alto de 'nibble' al LCD.
//****************************************************************************
void lcd_nibble(char nibble)
{
    lcd_CTRLout &= ~(1<<lcd_RW);     // Establecer Modo de escritura
    lcd_DATAddr |= 0xF0;             // Hacer nibble alto de bus de datos salida
    lcd_DATAout = (nibble&0xF0)|(lcd_DATAout&0x0F); // Colocar dato
    _delay_us(5);                     // tAS, set-up time > 140 ns
    lcd_CTRLout |= (1<<lcd_E);       // Pulso de Enable
    _delay_us(5);                     // Enable pulse width > 450 ns
    lcd_CTRLout &= ~(1<<lcd_E);      // 
    lcd_DATAddr &= 0x0F;             // Hacer nibble alto entrada
}

//****************************************************************************
// Lee un byte de dato del LCD.
// Si RS = 1 se lee la locaci�n de DDRAM o CGRAM direccionada actualmente.
// Si RS = 0 se lee el 'bit de Busy Flag' + el 'Puntero de RAM'.
//****************************************************************************
char lcd_read(char RS)
{
    char high, low;
    if(RS)
        lcd_CTRLout |= (1<<lcd_RS);  // Para leer de DDRAM o CGRAM
    else
        lcd_CTRLout &= ~(1<<lcd_RS); // Para leer Busy Flag + Puntero de RAM
    lcd_CTRLout |= (1<<lcd_RW);      // Establecer Modo Lectura
    lcd_DATAddr &= 0x0F;             // Hacer nibble alto entrada    
    _delay_us(2);                     // tAS, set-up time > 140 ns
    lcd_CTRLout |= (1<<lcd_E);       // Habilitar LCD
    _delay_us(2);                     // Data Delay Time > 1320 ns
    high = lcd_DATAin;               // Leer nibble alto
    lcd_CTRLout &= ~(1<<lcd_E);      // Para que el LCD prepare el nibble bajo
    _delay_us(2);                     // Enable cycle time > 1200 ns
    lcd_CTRLout |= (1<<lcd_E);       // Habilitar LCD
    _delay_us(2);                     // Data Delay Time > 1320 ns
    low = lcd_DATAin;                // Leer nibble bajo
    lcd_CTRLout &= ~(1<<lcd_E);      // 
    return (high&0xF0)|(low>>4);     // Juntar nibbles le�dos
}


//****************************************************************************
// Env�an cadenas RAM terminadas en nulo al LCD.
//****************************************************************************
void lcd_puts(char * s)
{
    unsigned char c, i=0;
    while(c = s[i++])
        lcd_write(c, 1);         // Instrucci�n 'Write Data to DDRAM/CGRAM'
}

//****************************************************************************
// Ubica el cursor del LCD en la columna c de la l�nea r.
//****************************************************************************
void lcd_gotorc(char r, char c)
{
    if(r==1)
	{
     r = LCD_LINE1;
	}	 
    else if (r==2)
	{
	  r = LCD_LINE2;
	}
	else if (r==3)
	{
	  r = LCD_LINE3;
	}
	else
	{
	 r = LCD_LINE4;	
	}
		  
    lcd_write(r+c-1, 0);         // Instrucci�n 'Set DDRAM Address'
}

//****************************************************************************
// Limpia la pantalla del LCD y regresa el cursor a la primera posici�n 
// de la l�nea 1.
//****************************************************************************
void lcd_clear(void)
{
    lcd_write(LCD_CLEAR, 0);     // Instrucci�n 'Clear Display'
}

//****************************************************************************
// Env�an instrucciones de comando y de datos al LCD.
//****************************************************************************
void lcd_cmd(char com)
{
    lcd_write(com, 0);           // Cualquier instrucci�n de comando
}
void lcd_data(char dat)
{
    lcd_write(dat, 1);           // Instrucci�n 'Write Data to DDRAM/CGRAM'
}

//****************************************************************************
// Genera un delay de n milisegundos
//****************************************************************************
void ldelay_ms(unsigned char n)
{
    while(n--)
        _delay_us(1000);
}
 
void lcd_custom_char(uint8_t location, uint8_t *charmap){
	if(location>=8){
		return;
	}
	lcd_cmd(0x40 | (location<<3));
	for(int i=0; i<8;i++){
		lcd_data(charmap[i]);
	}
}