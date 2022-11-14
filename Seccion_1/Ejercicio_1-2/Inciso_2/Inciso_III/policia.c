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
    long int darPermiso, recPedido, confirm;
    long int noBloqueante;
    int countSur, countNorte;
    Mensaje msg, msgTimer;
    char dirActual[20];
    
    countSur=0;
    countNorte=0;
    
    //Policia empieza recibiendo pedidos de SUR
    recPedido = PEDIDO_PERMISO_S;
    darPermiso = PERMISO_CONCEDIDO_S;
    confirm = ENTRA_S;
    strcpy(dirActual,STR_SUR);
    
    
    //Vincularse a cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR al vincularse a cola Policia: %s\n",strerror(errno));
    }
    
    //Policia empie
    
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
        
        printf("-------POLICIA: ahora es turno del %s------- \n",dirActual);
        
        while(!vaciaPuente){
            //EL policia intenta recibir pedidos de permiso de la direccion que tiene el permiso actualmente
            //Si recibe un pedido de permiso, envia el mensaje correspondiente para que el auto entre al puente
            //y espera por su confirmacion
            result = msgrcv(idCola, &msg, TAM_MSG_BYTES,recPedido,IPC_NOWAIT);
            if(result>=0){
                printf("Policia da permiso a auto de %s\n",dirActual);
                msg.tipo = darPermiso;
                if(msgsnd(idCola, &msg, TAM_MSG_BYTES,0)==-1){
                    printf("ERROR policia al conceder permiso a auto: %s\n",strerror(errno));
                }
                
                if(msgrcv(idCola, &msg, TAM_MSG_BYTES, confirm,0)==-1){
                    printf("ERROR policia al recibir confirmacion de entrada de auto: %s\n",strerror(errno));
                }
                
                if(msg.tipo==ENTRA_N){
                    countNorte++;
                }else if(msg.tipo==ENTRA_S){
                    countSur++;
                }else{
                    printf("ERROR policia recibio tipo de mensaje incorrecto \n");
                }
            }
            
            //Policia revisa si el timer envio mensaje para cambiar
            result = msgrcv(idCola, &msgTimer,TAM_MSG_BYTES,SWAP_POLI,IPC_NOWAIT);
            if(result>=0){
                vaciaPuente=1;
                waitpid(pid,NULL,0);
            }
        }
        
        printf("-------POLICIA: se acabo turno del %s. Esperando que se vacie el puente-------\n",dirActual);
        
        //Cuenta los autos que salieron y espera que los que todavia esten en el puente salgan
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
        
        
        //Cambia la direccion a la cual darle permiso
        if(recPedido==PEDIDO_PERMISO_N){
            recPedido=PEDIDO_PERMISO_S;
            darPermiso=PERMISO_CONCEDIDO_S;
            confirm = ENTRA_S;
            strcpy(dirActual,STR_SUR);
        }else if(recPedido==PEDIDO_PERMISO_S){
            recPedido=PEDIDO_PERMISO_N;
            darPermiso=PERMISO_CONCEDIDO_N;
            confirm = ENTRA_N;
            strcpy(dirActual,STR_NORTE);
        }
    }
   
    return 0;
}
