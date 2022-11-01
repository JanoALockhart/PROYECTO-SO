#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "utils.h"

int main(int argc, char *argv[]){
    int idCola;
    Mensaje cambiar;
    
    //Vincularse a la cola de mensajes
    idCola = msgget(KEY,0666);
    if(idCola<0){
        printf("ERROR al vincularse cola en timer: %s\n",strerror(errno));
    }
    
    sleep(TIEMPO_TIMER);
    
    cambiar.tipo=SWAP_POLI;
    if(msgsnd(idCola,&cambiar,TAM_MSG_BYTES,0)==-1){
        printf("ERROR al enviar mensaje de cambio al Policia: %s\n",strerror(errno));
    }
    
    return 0;
}
