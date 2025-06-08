 

 /******************************************************************************
 * Nombre Fichero:   lcd.c
 * Proposito:  Librería de funciones para controlar un display LCD con chip
 *             Hitachi HD44780 o compatible. La interface es de 4 bits.
 * Procesador:  ATmel AVR
 * Compilador:  IAR-C y AVR-GCC (WinAVR)
 * Author:     Shawn Johnson. http://www.cursomicros.com.
 *
 *****************************************************************************/

#include "lcd.h"

//****************************************************************************
// Ejecuta la inicialización software completa del LCD. La configuración es
// de: interface de 4 bits, despliegue de 2 líneas y caracteres de 5x7 puntos.
//****************************************************************************
void lcd_init(void)
{
    /* Configurar las direcciones de los pines de interface del LCD */
    lcd_DATAddr |= 0xF0;
    lcd_CTRLddr |= (1<<lcd_E)|(1<<lcd_RW)|(1<<lcd_RS);

    /* Secuencia de inicialización del LCD en modo de 4 bits*/
    lcd_CTRLout &= ~((1<<lcd_E)|(1<<lcd_RW)|(1<<lcd_RS));
    _delay_ms(45);         // > 40 ms
    lcd_nibble(0x30);      // Function Set: 8-bit
    _delay_ms(5);          // > 4.1 ms
    lcd_nibble(0x30);      // Function Set: 8-bit
    _delay_ms(1);          // > 100 µs
    lcd_nibble(0x30);      // Function Set: 8-bit
    _delay_ms(1);          // > 40 µs
    lcd_nibble(0x20);      // Function Set: 4-bit
    _delay_ms(1);          // > 40 µs
    lcd_nibble(0x20);      // Function Set: 4-bit, 2lines, 4×7font //0x20
    lcd_nibble(0x80);      //80
    lcd_write(0x0C, 0); // Display ON/OFF Control: Display on, Cursor off, Blink off
    lcd_write(0x01, 0);    // Clear Display
    lcd_write(0x06, 0);    // Entry Mode Set
}

//****************************************************************************
// Escribe una instrucción en el LCD:
// Si RS = 0 la instrucción es de comando (Function Set, Entry Mode set, etc).
// Si RS = 1 la instrucción es de dato y va a la DDRAM o CGRAM.
//****************************************************************************
void lcd_write(char inst, char RS)
{
    while(lcd_read(0)&0x80);         // Esperar mientras LCD esté ocupado
    if(RS)
        lcd_CTRLout |= (1<<lcd_RS);  // Para escribir en DDRAM o CGRAM
    else
        lcd_CTRLout &= ~(1<<lcd_RS); // Para escribir en Registro de Comandos
    _delay_us(5);                     // Permite actualizar el Puntero de RAM
    lcd_nibble(inst);                // Enviar nibble alto
    lcd_nibble(inst<<4);             // Enviar nibble bajo
}

//****************************************************************************
// Envía el nibble alto de 'nibble' al LCD.
//****************************************************************************
void lcd_nibble(char nibble)
{
    lcd_CTRLout &= ~(1<<lcd_RW);     // Establecer Modo de escritura
    lcd_DATAddr |= 0xF0;             // Hacer nibble alto de bus de datos salida
    lcd_DATAout = (nibble&0xF0)|(lcd_DATAout&0x0F); // Colocar dato
    _delay_us(2);                     // tAS, set-up time > 140 ns
    lcd_CTRLout |= (1<<lcd_E);       // Pulso de Enable
    _delay_us(2);                     // Enable pulse width > 450 ns
    lcd_CTRLout &= ~(1<<lcd_E);      // 
    lcd_DATAddr &= 0x0F;             // Hacer nibble alto entrada
}

//****************************************************************************
// Lee un byte de dato del LCD.
// Si RS = 1 se lee la locación de DDRAM o CGRAM direccionada actualmente.
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
    return (high&0xF0)|(low>>4);     // Juntar nibbles leídos
}


//****************************************************************************
// Envían cadenas RAM terminadas en nulo al LCD.
//****************************************************************************
void lcd_put(const char s){
	lcd_write(s, 1);
}

void lcd_puts(const char * s)
{
    unsigned char c, i=0;
    while(c = s[i++])
        lcd_write(c, 1);         // Instrucción 'Write Data to DDRAM/CGRAM'
}

//****************************************************************************
// Ubica el cursor del LCD en la columna c de la línea r.
//****************************************************************************
void lcd_gotorc(char r, char c)
{
	
	if(r==1) 
	{
     r = LCD_LINE1;
	}	 
     else if(r==2)
	{ 
     r = LCD_LINE2;// Instrucción 'Set DDRAM Address'
	} 
	 else if(r==3)
	{ 
     r = LCD_LINE3;// Instrucción 'Set DDRAM Address'
	} 
	 else 
	{ 
     r = LCD_LINE4;// Instrucción 'Set DDRAM Address'
	}            
	lcd_write(r+c-1, 0);
}

//****************************************************************************
// Limpia la pantalla del LCD y regresa el cursor a la primera posición 
// de la línea 1.
//****************************************************************************
void lcd_clear(void)
{
    lcd_write(LCD_CLEAR, 0);     // Instrucción 'Clear Display'
}

//****************************************************************************
// Envían instrucciones de comando y de datos al LCD.
//****************************************************************************
void lcd_cmd(char com)
{
    lcd_write(com, 0);           // Cualquier instrucción de comando
}
void lcd_data(char dat)
{
    lcd_write(dat, 1);           // Instrucción 'Write Data to DDRAM/CGRAM'
}

//****************************************************************************
// Genera un delay de n milisegundos
//****************************************************************************
/*
void _delay_ms(unsigned char n)
{
    while(n--)
        _delay_us(1000);
}
*/
