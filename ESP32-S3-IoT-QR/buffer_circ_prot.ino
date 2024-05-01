/**
 @file buffer_circ_prot.ino
*/

#include "buffer_circ_prot.h"

/**
 @brief isEmpty. Función para saber si el Buffer está vacío
 @param  buff. Puntero al buffer circular que se comprueba
 @return bool. true --> buffer vacio ; false --> buffer con elementos
*/
bool isEmpty(Buffer_Circ * buff)
{
	if(buff->contador == 0)
  {
		return true;
	}
  else
  {
		return false;
	}	
}

/**
 @brief isFull. Función para saber si el Buffer está lleno
 @param  buff. Puntero al buffer circular que se comprueba
 @return bool. true --> buffer lleno ; false --> buffer incompleto
*/
bool isFull(Buffer_Circ * buff)
{
	if(buff->contador == BUFSIZE)
  {
		return true;
	}
  else
  {
		return false;
	}	
}

/**
 @brief get_item. Funcion para obtener elemento del Buffer
 @param  buff. Puntero al buffer circular que se comprueba
 @param  data. Variable en la que se guarda el elemento obtenido
 @return int. 0 --> ejecucion sin errores ; -1 --> buffer vacio
*/
int get_item(String data, Buffer_Circ * buff )
{
	if(isEmpty(buff))
  {
		Serial.printf("Error: No hay datos en el vector\n"); 
		return -1;
	}
	else
  {
    portENTER_CRITICAL (&(buff->taskMux));
		data = buff->datos[buff->bufOUT];
		buff->contador--;
		//Aritmetica en módulo del índice del vector
		buff->bufOUT = (buff->bufOUT+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); 
    Serial.printf("Sacas dato de posicion %d\n",  buff->bufOUT);
		return 0;	
	}
}

/**
 @brief put_item. Función para introducir elemento en el Buffer
 @param  buff. Puntero al buffer circular que se comprueba
 @param  data. Variable con el elemento a introducir
 @return int. 0 --> ejecucion sin errores ; -1 --> buffer lleno
*/
int put_item(String data, Buffer_Circ * buff )
{
	if(isFull(buff))
  {
		Serial.printf("Error: El vector esta lleno\n"); 
		return -1;
	}
	else
  {
    portENTER_CRITICAL (&(buff->taskMux));
		buff->datos[buff->bufIN]=data;
		buff->contador++;
		//Aritmetica en módulo del índice del vector
		buff->bufIN = (buff->bufIN+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); 
    Serial.printf("Meto dato, en posicion %d\n", buff->bufIN);
		return 0;	
	}	
}

/**
 @brief number. Función para saber cuántos elementos tiene el Buffer
 @param  buff. Puntero al buffer circular que se comprueba
 @return int. Numero de elementos del buffer
*/
int number(Buffer_Circ * buff)
{
	return buff->contador;	
}

/**
 @brief listBuffer. Función para listar el contenido del Buffer
 @param  buff. Puntero al buffer circular a listar
 @return int. 
*/
int listBuffer(Buffer_Circ * buff)
{
	Serial.printf("Tu buffer contiene: ");
	for(int i = 0; i < BUFSIZE; i++)
  {
		Serial.printf("\t%d", buff->datos[i]);
	}
	Serial.printf("\n");
}

/*** End of file ****/