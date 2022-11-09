//--MACROS PARA LA PLANTA DE RECICLAJE--//
#ifndef __MACROS_PLANTA__INCLUDED
#define __MACROS_PLANTA__INCLUDED

#define LEN_BASURA 20 //Cantidad maxima de caracteres para el nombre de la basura

//Basura
typedef struct basura{
	char tipo[LEN_BASURA];
} basura;

#define CANT_REC 3 //Es la cantidad de recolectores en la planta
#define CANT_CLAS 2 //Es la cantidad de recicladores en la planta

/* VEL_GEN_BASURA
 * Es la cantidad de segundos que hay entre dos generaciones de basura
 * sucesivas de un recolector. (Si es 0, la generacion es instantanea.
 * A mayor numero, generaran basura mas lento)
 * 
 */
#define VEL_GEN_BASURA 3

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

//Nombres de la basura
#define STR_VIDRIO "vidrio"
#define STR_PLASTICO "plastico"
#define STR_CARTON "carton"
#define STR_ALUMINIO "aluminio"


#endif //__MACROS_PLANTA__INCLUDED
