#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils.h"


int main(){
    int idColaMsg, pid;
    Mensaje *msg;
    
    //Creamos cola de mensajes
    idColaMsg = msgget(KEY, IPC_CREAT| 0666);
    if(idColaMsg<0){
        printf("ERROR al crear cola de mensaje: %s\n",strerror(errno));
    }
    
    //Crear proceso hijo Policia
    pid = fork();
    if(pid==0){
        if(execv(POLICIA_FILE,NULL)==-1){
            printf("ERROR al cargar imagen de Policia\n");
        }
        exit(EXIT_FAILURE);
    }else if(pid<0){
        printf("Error al crear proceso policia: %s\n",strerror(errno));
    }
    
    //Creamos procesos hijos generadores
    //Generador Sur
    pid = fork();
    if(pid==0){
        char *args[] = {GEN_AUTOS_NAME,STR_SUR,NULL};
    
        if(execv(GEN_AUTOS_FILE,args)==-1){
            printf("ERROR al cargar imagen del generadorAutos SUR: %s\n",strerror(errno));
        }
        exit(EXIT_FAILURE);
    }else if(pid<0){
        printf("Error al crear generador Sur: %s\n",strerror(errno));
    }
    
    //Generador de Norte
    pid = fork();
    if(pid==0){
        char *args[] = {GEN_AUTOS_NAME,STR_NORTE,NULL};
    
        if(execv(GEN_AUTOS_FILE,args)==-1){
            printf("ERROR al cargar imagen del generadorAutos NORTE: %s\n",strerror(errno));
        }
        exit(EXIT_FAILURE);
    }else if(pid<0){
        printf("Error al crear generador Norte: %s\n",strerror(errno));
    }
    
    //Esperamos por todos los procesos
    for(int i=0; i<3; i++){
        wait(NULL);
    }

    return 0;
}
