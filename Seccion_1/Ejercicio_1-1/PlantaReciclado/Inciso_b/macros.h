#include <sys/types.h>

//--MACROS PARA LA PLANTA DE RECICLAJE--//
#ifndef __MACROS_PLANTA__INCLUDED
#define __MACROS_PLANTA__INCLUDED

//macros para la cola de mensajes
#define KEY (key_t) 3333
#define BYTES_MSG sizeof(mensaje)-sizeof(long)

//Cantidad maxima de caracteres para el nombre de la basura
#define LEN_BASURA 20 


//DEFINICION DE MENSAJE
typedef struct mensaje{
	long tipo;
	char msg[LEN_BASURA];
} mensaje;


#define CANT_REC 3 //Es la cantidad de recolectores en la planta
#define CANT_CLAS 2 //Es la cantidad de recicladores en la planta

/* VEL_GEN_BASURA
 * Es la cantidad de segundos que hay entre dos generaciones de basura
 * sucesivas de un recolector. (Si es 0, la generacion es instantanea.
 * A mayor numero, generaran basura mas lento)
 * 
 */
#define VEL_GEN_BASURA 5

/* VEL_CLASIFICACION_BASURA
 * Es la cantidad de segundos que un recolector tarda en clasificar la basura.
 * Una vez pasado ese tiempo, puede clasificar otra basura
 * (Si es 0, es instantaneo. A mayor numero, mas tarda en clasificar)
 * 
 */
 #define VEL_CLASIFICACION_BASURA 2
 
 /* VEL_RECICLADO_BASURA
  * Es la cantidad de segundos que un reciclador tarda en reciclar la
  * basura. Una vez pasado ese tiempo, puede reciclar otra basura.
  * (0 es instantaneo. A mayor numero, mas tarda en reciclar)
  */
#define VEL_RECICLADO_BASURA 5

/* VEL_TOMANDO_MATE
 * Indica el tiempo que tarda un reciclador en tomar mate
 * */
#define VEL_TOMANDO_MATE 3

//Tipos de basura (mensaje)
#define VIDRIO 1
#define CARTON 2
#define PLASTICO 3
#define ALUMINIO 4
#define BASURA_RECOLECTADA 10


//Nombres de la basura
#define STR_VIDRIO "vidrio"
#define STR_PLASTICO "plastico"
#define STR_CARTON "carton"
#define STR_ALUMINIO "aluminio"

//nombre archivos
#define RECOLECTOR_FILE "./recolector"
#define RECOLECTOR_NAME "recolector"

#define CLASIFICADOR_FILE "./clasificador"
#define CLASIFICADOR_NAME "clasificador"

#define RECICLADOR_FILE "./reciclador"
#define RECICLADOR_NAME "reciclador"

#endif //__MACROS_PLANTA__INCLUDED
