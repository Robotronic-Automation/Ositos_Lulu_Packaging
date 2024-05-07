/**
 @file buffer_circ_prot.h
*/

#ifndef BUFFER_CIRC_PROT_H
#define BUFFER_CIRC_PROT_H

#include "src/quirc/quirc.h"
#define BUFSIZE 10
#define true 1
#define false 0

#define T_espera 1000

/**
 @brief  Buffer_Circ. Estructura para instanciar un buffer circular protegido
 @member datos[BUFSIZE]. Vector de strings que almacena los datos del buffer
 @member bufIN. Posicion de entrada del proximo elemento
 @member bufOUT. Posicion de salida del proximo elemento 
 @member contador. Variable que almacena el numero de elementos del buffer
 @member taskMux. Mutex para proteger el acceso a los elementos del buffer
*/
typedef struct Buffer 
{
	char datos[10][BUFSIZE];
	int bufIN = 0;
  int bufOUT = 0;
  int contador = 0;
  portMUX_TYPE taskMux = portMUX_INITIALIZER_UNLOCKED;
} Buffer_Circ;

// Función para saber si el Buffer está vacío
bool isEmpty(Buffer_Circ * buff);

// Función para saber si el Buffer está lleno
bool isFull(Buffer_Circ * buff);

// Funcion para obtener elemento del Buffer
int get_item(char data[], Buffer_Circ * buff );

// Función para introducir elemento en el Buffer
int put_item(char data[], Buffer_Circ * buff );

// Función para saber cuántos elementos tiene el Buffer
int number(Buffer_Circ * buff);

// Función para listar el contenido del Buffer
int listBuffer(Buffer_Circ * buff);

#endif 

/*** End of file ****/