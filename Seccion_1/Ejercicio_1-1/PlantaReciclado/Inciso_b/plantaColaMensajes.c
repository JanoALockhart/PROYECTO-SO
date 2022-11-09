#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

#include "macros.h"

//PLANTA RECICLADO CON COLA DE MENSAJES
int main(){
    int pid, colaMsgId, result;
    mensaje m;
    
    //Creamos cola de mensajes
    colaMsgId = msgget(KEY,IPC_CREAT|0666);
    if(colaMsgId<0){
        printf("Error al crear cola de Mensajes de Recolectores a Clasificadores\n");
        printf("%s\n",strerror(errno));
    }
    
    //Creamos Clasificadores
    for(int i=0; i<CANT_CLAS; i++){
        
        pid = fork();
        if(pid<0){
            printf("Error al crear Clasificador: %s\n",strerror(errno));
            
        }else if(pid==0){
            char num[5];
            sprintf(num,"%d",i);
            char *argv[] = {CLASIFICADOR_NAME,num,NULL};
        
            if(execv(CLASIFICADOR_FILE,argv)==-1){
                printf("ERROR al cargar imagen de: %s\n",strerror(errno));
            }
            exit(EXIT_FAILURE);
        }
    }  
    
    
    //--Creamos recicladores--//
    //Reciclador de vidrio
    pid = fork();
    if(pid<0){
        printf("Error al crear reciclador de vidrio: %s\n",strerror(errno));
        
    }else if(pid==0){
        char tipoBas[5];
        sprintf(tipoBas,"%d",VIDRIO);
        char *argv[] = {RECICLADOR_NAME,tipoBas,STR_VIDRIO,NULL};
        
        if(execv(RECICLADOR_FILE,argv)==-1){
            printf("ERROR al cargar imagen de: %s\n",strerror(errno));
        }
        exit(EXIT_FAILURE);
    }  
    
    //Reciclador de carton
    pid = fork();
    if(pid<0){
        printf("Error al crear reciclador de carton: %s\n",strerror(errno));
        
    }else if(pid==0){
        char tipoBas[5];
        sprintf(tipoBas,"%d",CARTON);
        char *argv[] = {RECICLADOR_NAME,tipoBas,STR_CARTON,NULL};
        
        if(execv(RECICLADOR_FILE,argv)==-1){
            printf("ERROR al cargar imagen de: %s\n",strerror(errno));
        }
        exit(EXIT_FAILURE);
    } 
    
    //Reciclador de plastico
    pid = fork();
    if(pid<0){
        printf("Error al crear reciclador de plastico: %s\n",strerror(errno));
        
    }else if(pid==0){
        char tipoBas[5];
        sprintf(tipoBas,"%d",PLASTICO);
        char *argv[] = {RECICLADOR_NAME,tipoBas,STR_PLASTICO,NULL};
        
        if(execv(RECICLADOR_FILE,argv)==-1){
            printf("ERROR al cargar imagen de: %s\n",strerror(errno));
        }
        exit(EXIT_FAILURE);
    } 
    
    //Reciclador de aluminio
    pid = fork();
    if(pid<0){
        printf("Error al crear reciclador de aluminio: %s\n",strerror(errno));
        
    }else if(pid==0){
        char tipoBas[5];
        sprintf(tipoBas,"%d",ALUMINIO);
        char *argv[] = {RECICLADOR_NAME,tipoBas,STR_ALUMINIO,NULL};
        
        if(execv(RECICLADOR_FILE,argv)==-1){
            printf("ERROR al cargar imagen de: %s\n",strerror(errno));
        }
        exit(EXIT_FAILURE);
    } 
    
        
    //Creamos recolectores
    for(int i=0; i<CANT_REC; i++){
        
        pid = fork();
        if(pid<0){
            printf("Error al crear recolector\n");
            
        }else if(pid==0){
            char num[5];
            sprintf(num,"%d",i);
            char *argv[] = {RECOLECTOR_NAME,num,NULL};
        
            if(execv(RECOLECTOR_FILE,argv)==-1){
                printf("ERROR al cargar imagen de recolector: %s\n",strerror(errno));
            }
            exit(EXIT_FAILURE);
        }
    }
    
    //El padre espera por todos los procesos creados
    for(int i=0; i<7; i++){
        wait(NULL);
    }
    

    return 0;
}
