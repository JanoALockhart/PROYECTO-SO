#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#include "macros.h"


//argv[0] = reciclador
//argv[1] = tipo de mensaje (basura) que recibe
//argv[2] = nombre de la basura que recibe
int main(int argc, char *argv[]){
    int ayudar, idCola, result;
    long tipoBasura; 
    mensaje m;
    
    tipoBasura = strtol(argv[1],NULL,10);
    
    //Vincularse a la cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR recolector al vincularse a cola: %s\n",strerror(errno));
    }
    
    while(1){
        //Primero revisa si puede recibir basura del tipo que le corresponde
        ayudar = msgrcv(idCola, &m, BYTES_MSG,tipoBasura,IPC_NOWAIT);
        if(ayudar>0){
            printf("Rec. %s: reciclando...\n",argv[2]);
            sleep(VEL_RECICLADO_BASURA);
            printf("Rec. %s: recicla [%ld,%s]\n",argv[2],m.tipo,m.msg);
            
            
        }else if (ayudar ==-1 & errno==ENOMSG){//Si hay error, no encontro mensaje de su tipo entonces...
            ayudar = msgrcv(idCola, &m, BYTES_MSG,-4,IPC_NOWAIT);//intenta ayudar otro
            if(ayudar>0){
                printf("Rec. %s: reciclando...\n",argv[2]);
                sleep(VEL_RECICLADO_BASURA);
                if(m.tipo==tipoBasura){
                    printf("Rec. %s: recicla [%ld,%s]\n",argv[2],m.tipo,m.msg);//Se "ayuda" a si mismo
                }else{
                    printf("Rec. %s: Ayude reciclador de %s con [%ld,%s]\n",argv[2],m.msg,m.tipo,m.msg);//Ayuda a otro
                }
                
            }else if(ayudar ==-1 & errno==ENOMSG){//Sino se pone a tomar mate
                printf("Rec. %s: Nadie para ayudar. Tomando unos mates...\n",argv[2]);                    
                sleep(VEL_TOMANDO_MATE);
                
            }else{
                printf("ERROR en reciclador: %s\n",strerror(errno));                    
            }
        }
    }

    return 0;
}
