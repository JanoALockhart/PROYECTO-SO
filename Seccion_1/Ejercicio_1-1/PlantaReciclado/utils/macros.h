//--MACROS PARA LA PLANTA DE RECICLAJE--//
#ifndef __MACROS_PLANTA__INCLUDED
#define __MACROS_PLANTA__INCLUDED


#define CANT_REC 3 //Es la cantidad de recolectores en la planta
#define CANT_CLAS 2 //Es la cantidad de recicladores en la planta

/* VEL_CREACION_RECOLECTORES
 * Es la cantidad de segundos que hay entre la creacion de dos recolectores.
 * Este debe ser distinto de 0 para que los recolectores generen tipos de
 * basura distintos ya que utilizan la funcion rnd() en base al tiempo en
 * el que fue invocada. (Si es 0, la creacion es instantanea. A mayor número,
 * más tiempo tardaran en crearse)
 */
#define VEL_CREACION_RECOLECTORES 1

/* VEL_GEN_BASURA
 * Es la cantidad de segundos que hay entre dos generaciones de basura
 * sucesivas de un recolector. (Si es 0, la generacion es instantanea.
 * A mayor numero, generaran basura mas lento)
 * 
 */
#define VEL_GEN_BASURA 10

/* VEL_CLASIFICACION_BASURA
 * Es la cantidad de segundos que un recolector tarda en clasificar la basura.
 * Una vez pasado ese tiempo, puede clasificar otra basura
 * (Si es 0, es instantaneo. A mayor numero, mas tarda en clasificar)
 * 
 */
 #define VEL_CLASIFICACION_BASURA 0
 
 /* VEL_RECICLADO_BASURA
  * Es la cantidad de segundos que un reciclador tarda en reciclar la
  * basura. Una vez pasado ese tiempo, puede reciclar otra basura.
  * (0 es instantaneo. A mayor numero, mas tarda en reciclar)
  */
#define VEL_RECICLADO_BASURA 0

/* VEL_TOMANDO_MATE
 * Indica el tiempo que tarda un reciclador en tomar mate
 * */
#define VEL_TOMANDO_MATE 5

//Tipos de basura
#define VIDRIO 2
#define CARTON 3
#define PLASTICO 4
#define ALUMINIO 5

//Estado de la basura
#define BASURA_RECOLECTADA 1


#endif //__MACROS_PLANTA__INCLUDED
