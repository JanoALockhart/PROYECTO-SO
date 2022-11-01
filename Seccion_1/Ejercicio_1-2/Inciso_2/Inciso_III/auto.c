#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

int main(int argc, char *argv[]){
    int idCola;
    long int permiso, entrada, salida;
    Mensaje msg;
    
    //Asociarse a la cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR al asociarse a la cola de mensajes: %s\n",strerror(errno));
    }
    
    //Asignar los tipos de mensaje que debe recibir/enviar
    if(strcmp(argv[1],STR_NORTE)==0){
        permiso = PERMISO_N;
        entrada = ENTRA_N;
        salida = SALE_N;
    }else if(strcmp(argv[1],STR_SUR)==0){
        permiso = PERMISO_S;
        entrada = ENTRA_S;
        salida = SALE_S;
    }else{
        printf("ERROR direccion no valida para auto\n");
    }
    
    printf("Auto de %s APARECIO\n",argv[1]);
    //Protocolo de entrada
    if(msgrcv(idCola,&msg,TAM_MSG_BYTES,permiso,0)==-1){ //Recibir permiso de policia
        printf("ERROR al leer mensaje Policia-Auto: %s\n",strerror(errno));
    }
    
    /* NO NECESARIO SI SE ASUME QUE SIEMPRE HAY AUTOS QUE TOMEN LOS PERMISOS DEL POLICIA
    msg.tipo = entrada;
    
    if(msgsnd(idCola,&msg,TAM_MSG_BYTES,0)==-1){ //Decirle al policia que entra
        printf("ERROR al enviar mensaje Auto-Policia: %s\n",strerror(errno));
    }*/
    
    //Seccion critica
    printf("Auto %s ENTRA al puente\n",argv[1]);
    printf("Auto %s PASANDO por el puente\n",argv[1]);
    sleep(TIEMPO_EN_PUENTE);
    printf("Auto %s SALE del puente\n",argv[1]);
    
    //Protocolo de salida
    msg.tipo = salida;
    if(msgsnd(idCola,&msg,TAM_MSG_BYTES,0)==-1){//Decirle al policia que sale
        printf("ERROR al enviar mensaje Auto-Policia: %s\n",strerror(errno));
    }
    

    return 0;
}


