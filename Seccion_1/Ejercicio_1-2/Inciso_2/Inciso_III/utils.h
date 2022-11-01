#include <sys/types.h>

//Mensaje
typedef struct{
    long int tipo;
} Mensaje;


//Macros para cola de mensajes
#define KEY (key_t) 1111
#define TAM_MSG_BYTES sizeof(Mensaje)-sizeof(long)

#define STR_SUR "SUR"
#define STR_NORTE "NORTE"

//TIPOS DE MENSAJES
#define PERMISO_N 1
#define ENTRA_N 2
#define SALE_N 3

#define PERMISO_S 10
#define ENTRA_S 11
#define SALE_S 12

#define SWAP_POLI 20


//Nombre archivos
#define GEN_AUTOS_NAME "generadorAutos"
#define GEN_AUTOS_FILE "./generadorAutos"

#define AUTO_NAME "auto"
#define AUTO_FILE "./auto"

#define POLICIA_FILE "./policia"

#define TIMER_FILE "./timer"

//Tiempos
#define TIEMPO_APARICION_AUTOS 3
#define TIEMPO_EN_PUENTE 5
#define TIEMPO_ENTRE_PERMISOS 4
#define TIEMPO_TIMER 10


