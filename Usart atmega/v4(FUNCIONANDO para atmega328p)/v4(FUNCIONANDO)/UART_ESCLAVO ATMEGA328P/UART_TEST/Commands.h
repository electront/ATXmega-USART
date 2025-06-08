/*
 * Commands.h
 *
 * Created: 18/2/2025 22:16:41
 *  Author: Alvaro
 */ 


#ifndef COMMANDS_H_
#define COMMANDS_H_

#define COMIENZO_TRAMA	0x02

#define ENVIO_COMANDO 0x04

typedef enum{
		
	// ENVIO_COMANDO = 0x05,
	 
	 RESPUESTA = 0x25,
	 
	 CORRIENTE_1 = 0x26,
	 CORRIENTE_2 = 0x27,
	 CORRIENTE_3 = 0x28,
	 CORRIENTE_4 = 0x29,
	 
	 TEMPERATURA_1 = 0x30,
	 TEMPERATURA_2 = 0x31,
	 TEMPERATURA_3 = 0x32,
	 TEMPERATURA_4 = 0x33,
	 
	 POTENCIA = 0x34,
	 S_ROE = 0x35,
	 
	 S_FIN_DATOS = 0xFF,
	 
	 U_POTENCIA_30 = 0x36,
	 U_POTENCIA_60 = 0x37,
	 U_POTENCIA_100 = 0x38,
	 
	 U_EQUIPO_ON = 0x39,
	 U_EQUIPO_OFF = 0x40,
	 
	 U_CONEXION_INICIAL = 0X41,
	 
	 U_ROE_200 = 0x42,
	 u_ROE_400 = 0x43,
	 
	 U_FUENTE = 0x44,
	 
	 U_REINICIAR_PROTECCIONES = 0x45,
	 
	 U_POTENCIA_SALIDA_MAX_200 = 0x46,
	 U_POTENCIA_SALIDA_MAX_400 = 0x47,
	 
	 U_POTENCIA_ENTRADA_MAXIMA = 0x48,
	 
	 U_DESBALANCE_POTENCIA = 0x49,
	 
	 U_PREGUNTANDO_PUERTO = 0x50, //Lo uso para que la computadora sepa en que puerto esta conectado el amplificador.
	 U_PROTECCION_EXTERNA = 0x51,
	 
	 ERROR_TRAMA = 0x52,
	 
}Command;

#endif /* COMMANDS_H_ */