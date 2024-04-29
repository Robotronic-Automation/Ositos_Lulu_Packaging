/**
 @file buffer_circ_prot.ino
*/

#include "buffer_circ_prot.h"

//Función para saber si el Buffer está vacío
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

//Función para saber si el Buffer está lleno
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

//Funcion para obtener elemento del Buffer
int get_item(int * data, Buffer_Circ * buff )
{
	if(isEmpty(buff))
  {
		Serial.printf("Error: No hay datos en el vector\n"); 
		return -1;
	}
	else
  {
    portENTER_CRITICAL (&(buff->taskMux));
		*data = buff->datos[buff->bufOUT];
		buff->contador--;
		//Aritmetica en módulo del índice del vector
		buff->bufOUT = (buff->bufOUT+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); 
    Serial.printf("Sacas dato de posicion %d\n",  buff->bufOUT);
		return 0;	
	}
}

//Función para introducir elemento en el Buffer
int put_item(int data , Buffer_Circ * buff )
{
	if(isFull(buff))
  {
		Serial.printf("Error: El vector esta lleno\n"); 
		return -1;
	}
	else
  {
    portENTER_CRITICAL (&(buff->taskMux));
		buff->datos[buff->bufIN] = data;
		buff->contador++;
		//Aritmetica en módulo del índice del vector
		buff->bufIN = (buff->bufIN+1) % BUFSIZE;
    portEXIT_CRITICAL (&(buff->taskMux)); 
    Serial.printf("Meto dato, en posicion %d\n", buff->bufIN);
		return 0;	
	}	
}

//Función para saber cuántos elementos tiene el Buffer
int number(Buffer_Circ * buff)
{
	return buff->contador;	
}

//Función para listar el contenido del Buffer
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