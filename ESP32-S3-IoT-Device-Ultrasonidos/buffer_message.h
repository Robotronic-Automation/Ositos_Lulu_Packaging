/**
 * @file buffer_message.h
 */

#ifndef BUFFER_MESSAGE_H
#define BUFFER_MESSAGE_H

#include "src/quirc/quirc.h"
#define BUFSIZE 10
#define true 1
#define false 0

#define T_espera 1000

/**
 * @struct  Buffer_Circ. Estructura para instanciar un buffer circular protegido
 * @member datos[BUFSIZE]. Vector de strings que almacena los datos del buffer
 * @member bufIN. Posicion de entrada del proximo elemento
 * @member bufOUT. Posicion de salida del proximo elemento 
 * @member contador. Variable que almacena el numero de elementos del buffer
 * @member taskMux. Mutex para proteger el acceso a los elementos del buffer
 */
typedef struct Buffer_Message 
{
	char datos[10][BUFSIZE];
	int bufIN = 0;
  int bufOUT = 0;
  int contador = 0;
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED;
} Buffer_Circ_Message;

/**
 * @fn Función para saber si el Buffer está vacío
 */
bool isEmpty(Buffer_Circ_Message * buff);

/**
 * @fn Función para saber si el Buffer está lleno
 */
bool isFull(Buffer_Circ_Message * buff);

/**
 * @fn Funcion para obtener elemento del Buffer
 */
int get_item(char data[], Buffer_Circ_Message * buff );

/**
 * @fn Función para introducir elemento en el Buffer
 */
int put_item(char data[], Buffer_Circ_Message * buff );

/**
 * @fn Función para saber cuántos elementos tiene el Buffer
 */
int number(Buffer_Circ_Message * buff);

/**
 * @fn Función para listar el contenido del Buffer
 */
int listBuffer(Buffer_Circ_Message * buff);

#endif 

/*** End of file ****/