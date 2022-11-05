#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

/*
 * argv[0] = auto
 * argv[1] = direccion del auto (SUR/NORTE)
 * argv[2] = numero entero para identificarlo (int)
 * */
int main(int argc, char *argv[]){
    int idCola;
    long int pedirPermiso, permisoConc, entrada, salida;
    Mensaje msg;
    
    //Asociarse a la cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR al asociarse a la cola de mensajes: %s\n",strerror(errno));
    }
    
    //Asignar los tipos de mensaje que debe recibir/enviar
    if(strcmp(argv[1],STR_NORTE)==0){
        pedirPermiso = PEDIDO_PERMISO_N;
        permisoConc = PERMISO_CONCEDIDO_N;
        entrada = ENTRA_N;
        salida = SALE_N;
    }else if(strcmp(argv[1],STR_SUR)==0){
        pedirPermiso = PEDIDO_PERMISO_S;
        permisoConc = PERMISO_CONCEDIDO_S;
        entrada = ENTRA_S;
        salida = SALE_S;
    }else{
        printf("ERROR direccion no valida para auto\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Auto %s de %s APARECIO\n",argv[2],argv[1]);
    
    //---Protocolo de entrada---//
    //Pedir permiso a policia
    msg.tipo = pedirPermiso;
    if(msgsnd(idCola,&msg,TAM_MSG_BYTES,0)==-1){
        printf("ERROR auto al pedirle permiso a policia: %s\n",strerror(errno));
    }
    //Recibir permiso de policia
    if(msgrcv(idCola,&msg,TAM_MSG_BYTES,permisoConc,0)==-1){ 
        printf("ERROR al leer mensaje Policia-Auto: %s\n",strerror(errno));
    }
    //Enviar mensaje de confirmacion
    msg.tipo = entrada;
    if(msgsnd(idCola,&msg,TAM_MSG_BYTES,0)==-1){
        printf("ERROR auto al enviar mensaje de entrada a policia\n");
    }
    
    //---Seccion critica---//
    printf("Auto %s de %s ENTRA al puente\n",argv[2],argv[1]);
    sleep(TIEMPO_EN_PUENTE);
    printf("Auto %s de %s SALE del puente\n",argv[2],argv[1]);
    
    //---Protocolo de salida---//
    //Decirle al policia que sale del puente
    msg.tipo = salida;
    if(msgsnd(idCola,&msg,TAM_MSG_BYTES,0)==-1){
        printf("ERROR al enviar mensaje Auto-Policia: %s\n",strerror(errno));
    }

    return 0;
}


