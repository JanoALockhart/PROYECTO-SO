#include <stdio.h>
#include <stdlib.h>

#ifndef __GENERACION__BASURA__INCLUDED
#define __GENERACION__BASURA__INCLUDED

//MACROS
#define LEN_BASURA 20 //Cantidad maxima de caracteres para el nombre de la basura
#define BYTES_MSG sizeof(mensaje)-sizeof(long)

//DEFINICION DE TIPOS
//Mensaje de basura para Inciso b
typedef struct mensaje{
	long tipo;
	char msg[LEN_BASURA];
} mensaje;

//Basura
typedef struct basura{
	char tipo[LEN_BASURA];
} basura;

#endif //__GENERACION__BASURA__INCLUDED
