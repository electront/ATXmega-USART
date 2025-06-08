/******************************************************************************
 * FileName:   lcd.h
 * Purpose:    Librería de funciones para controlar un display LCD con chip
 *             Hitachi HD44780 o compatible. La interface es de 4 bits.
 * Processor:  ATmel AVR
 * Compiler:   IAR-C y AVR-GCC (WinAVR)
 * Author:     Shawn Johnson. http://www.cursomicros.com.
 *
 * Copyright (C) 2008 - 2012 Shawn Johnson. All rights reserved.
 *
 * License:    Se permiten el uso y la redistribución de este código con 
 *             modificaciones o sin ellas, siempre que se mantengan esta 
 *             licencia y las notas de autor y copyright de arriba.
 *****************************************************************************/

#include "compiler.h"
#include "delay.h"


//****************************************************************************
//    CONFIGURACIÓN DE LOS PINES DE INTERFACE
//****************************************************************************

/* Define el puerto a donde se conectará el bus de datos del LCD
 * Se utilizará el nible alto del puerto escogido (ejem. PB4-DB4,...,PB7-DB7)
 */
#define  lcd_DATAout   PORTD_OUT      // Registro PORT del puerto
#define  lcd_DATAin    PORTD_IN       // Registro PIN del puerto
#define  lcd_DATAddr   PORTD_DIR       // Registro DDR del puerto

// Define el puerto a donde se conectarán las líneas de control del LCD
// E, RW y RS. Puede ser el mismo puerto del bus de datos.

#define  lcd_CTRLout   PORTD_OUT      // Registro PORT del puerto
#define  lcd_CTRLin    PORTD_IN       // Registro PIN del puerto
#define  lcd_CTRLddr   PORTD_DIR       // Registro DDR del puerto

// Define los números de los pines del puerto anterior que corresponderán a 
// las líneas E, RW y RS del LCD.

#define  lcd_E         PIN3_bp         // Pin Enable
#define  lcd_RW        PIN2_bp          // Pin Read/Write
#define  lcd_RS        PIN1_bp          // Pin Register Select


//****************************************************************************
//    CÓDIGOS DE COMANDO USUALES
//****************************************************************************
#define  LCD_CLEAR    0x01    // Limpiar Display
#define  LCD_RETHOM   0x02    // Cursor a inicio de línea 1
#define  LCD_LINE1    0x80    // Línea 1 posición 0
#define  LCD_LINE2    0xC0    // Línea 2 posición 0
#define  LCD_LINE3    0x94    // Línea 3 posición 0 //94
#define  LCD_LINE4    0xD4    // Línea 4 posición 0 \\D4

#define  LCD_DDRAM    0x80    // Dirección 0x00 de DDRAM
#define  LCD_CGRAM    0x40    // Dirección 0x00 de CGRAM
#define  LCD_CURSOR   0x0E    // Mostrar solo Cursor
#define  LCD_BLINK    0x0D    // Mostrar solo Blink
#define  LCD_CURBLK   0x0F    // Mostrar Cursor + Blink
#define  LCD_NOCURBLK 0x0C    // No mostrar ni Cursor ni Blink

//****************************************************************************
//    PROTOTIPOS DE FUNCIONES
//****************************************************************************
void lcd_init(void);                // Inicializa el LCD
void lcd_puts(char * s);            // Envía una cadena ram al LCD
void lcd_gotorc(char r, char c);    // Cursor a fila r, columna c
void lcd_clear(void);           // Limpia el LCD y regresa el cursor al inicio
void lcd_data(char dat);            // Envía una instrucción de dato al LCD
void lcd_cmd(char com);             // Envía una instrucción de comando al LCD
char lcd_read(char RS);             // Lee un dato del LCD
void lcd_write(char inst, char RS); // Escribe una instrucción en el LCD
void lcd_nibble(char nibble);
void ldelay_ms(unsigned char );
void lcd_custom_char(uint8_t location, uint8_t *charmap);
 