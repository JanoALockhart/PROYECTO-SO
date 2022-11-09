#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/msg.h>

#include "macros.h"

/* Funcion: (void *) generarBasura(mensaje *m)
 * Entrada:
 *      -m: puntero a una struct de tipo mensaje.
 * Salida: ---
 * 
 * Genera aleatoriamente un nombre de basura para asignarselo
 * al campo msg de m.
 * Las basuras pueden ser: vidrio, carton, plastico, aluminio
 *         
 */
void* generarBasura(mensaje *m){
    srand(time(NULL)*getpid());
	int rgn = (rand()%4)+1;
    
	if(rgn==VIDRIO){
        strcpy(m->msg,STR_VIDRIO);
	}else if(rgn==CARTON){
        strcpy(m->msg,STR_CARTON);
    }else if(rgn==PLASTICO){
        strcpy(m->msg,STR_PLASTICO);
    }else if(rgn==ALUMINIO){
        strcpy(m->msg,STR_ALUMINIO);
    }
}

//argv[0] = recolector
//argv[1] = numero de recolector
int main(int argc, char *argv[]){
    int idCola, result;
    mensaje m;
    
    //Vincularse a la cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR recolector al vincularse a cola: %s\n",strerror(errno));
    }
    
    while(1){
        generarBasura(&m);
        m.tipo = BASURA_RECOLECTADA;
        printf("Recolector %s: envia [%ld,%s]\n",argv[1],m.tipo,m.msg);
                
        result = msgsnd(idCola, &m, BYTES_MSG,0);
        if (result<0){
            printf("Error en la escritura mensaje: %s\n",strerror(errno));
        }
        
        sleep(VEL_GEN_BASURA);//Intervalo de tiempo en el que no se genera basuras
    }
    
    
    return 0;
}

