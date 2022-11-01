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
    int vaciaPuente, pid, idCola,result;
    long int permiso;
    int countSur, countNorte;
    Mensaje msg, msgTimer;
    char dir[20];
    
    permiso = PERMISO_S;
    strcpy(dir,STR_SUR);
    countSur=0;
    countNorte=0;
    
    //Vincularse a cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR al vincularse a cola Policia: %s\n",strerror(errno));
    }
    
    
    while(1){
        vaciaPuente=0;
        
        //Policia crea timer
        pid = fork();
        if(pid==0){
            if(execv(TIMER_FILE,NULL)==-1){
                printf("ERROR al cargar imagen del timer\n");
            }
            exit(EXIT_FAILURE);
        }else if(pid<0){
            printf("ERROR al crear proceso hijo para el timer\n");
        }
        
        printf("-------POLICIA: ahora es turno del %s------- \n",dir);
        while(!vaciaPuente){
            //Dar permisos a autos de la direccion en permiso
            msg.tipo = permiso;
            if(msgsnd(idCola, &msg, TAM_MSG_BYTES, 0)==-1){
                printf("ERROR Policia al intentar enviar permiso a Auto: %s\n",strerror(errno));
            }
            if(permiso==PERMISO_S){
                countSur++;
            }else{
                countNorte++;
            }
            
            //Ve si el timer envio mensaje para cambiar
            result = msgrcv(idCola, &msgTimer,TAM_MSG_BYTES,SWAP_POLI,IPC_NOWAIT);
            if(result>=0){
                vaciaPuente=1;
                waitpid(pid,NULL,0);
            }

            sleep(TIEMPO_ENTRE_PERMISOS);
        }
        
        printf("-------POLICIA: se acabo turno del %s. Esperando que se vacie el puente-------\n",dir);
        
        //Espera a que los autos del puente salgan
        while(countSur>0){
            if(msgrcv(idCola,&msg,TAM_MSG_BYTES,SALE_S,0)==-1){
                printf("ERROR al esperar autos del SUR antes de cambiar: %s \n",strerror(errno));
            }
            countSur--;
        }
        
        while(countNorte>0){
            if(msgrcv(idCola,&msg,TAM_MSG_BYTES,SALE_N,0)==-1){
                printf("ERROR al esperar autos del NORTE antes de cambiar: %s \n",strerror(errno));
            }
            countNorte--;
        }
        
        //Cambia a quien darle permiso
        if(permiso == PERMISO_N){
            permiso = PERMISO_S;
            strcpy(dir,STR_SUR);
        }else{
            permiso = PERMISO_N;
            strcpy(dir,STR_NORTE);
        }
        
    }
   
    return 0;
}
