#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#include "macros.h"

//argv[0] = clasificador
//argv[1] = numero de clasificador
int main(int argc, char *argv[]){
    int idCola, result;
    mensaje m;
    
    //Vincularse a la cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR recolector al vincularse a cola: %s\n",strerror(errno));
    }

    while(1){
        //Recibimos basura de recolectores
        result = msgrcv(idCola, &m, BYTES_MSG, BASURA_RECOLECTADA, 0);
        if(result==-1){
            printf("ERROR clasificador al recibir mensaje de recolector: %s\n",strerror(errno));
        }
                
        printf("Clasificador %s: recibe [%ld,%s]\n",argv[1],m.tipo,m.msg);
                
        //La clasificamos
        if(strcmp(m.msg,STR_VIDRIO)==0){
            m.tipo=VIDRIO;
        }else if(strcmp(m.msg,STR_CARTON)==0){
            m.tipo=CARTON;
        }else if(strcmp(m.msg,STR_PLASTICO)==0){
            m.tipo=PLASTICO;
        }else if(strcmp(m.msg,STR_ALUMINIO)==0){
            m.tipo=ALUMINIO;
        }else{
            printf("ERROR: Basura no reconocida\n");
        }
                
        sleep(VEL_CLASIFICACION_BASURA);//Intervalo de tiempo que tarda en clasificar
        
        //Se envia la basura hacia los recicladores
        printf("Clasificador %s envia: [%ld,%s]\n",argv[1],m.tipo,m.msg);
        result = msgsnd(idCola, &m, BYTES_MSG, 0);
        if(result==-1){
            printf("Error al enviar a reciclador: %s\n",strerror(errno));
        }
                
    }
    
    return 0;
}


