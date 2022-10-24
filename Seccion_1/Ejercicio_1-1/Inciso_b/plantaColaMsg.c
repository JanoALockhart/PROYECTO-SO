#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include <time.h>

#include "../utils/macros.h"
#include "../utils/genBasura.h"

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
    srand(time(NULL));
	int rgn = (rand()%4)+2;
    
	if(rgn==VIDRIO){
        strcpy(m->msg,"vidrio");
	}else if(rgn==CARTON){
        strcpy(m->msg,"carton");
    }else if(rgn==PLASTICO){
        strcpy(m->msg,"plastico");
    }else if(rgn==ALUMINIO){
        strcpy(m->msg,"aluminio");
    }
}

/*
 * Funcion que contiene el código que realiza el recolector
 * queue: el la ID de la cola de mensajes en la que el recolector deposita la basura
 * nro: es el numero asignado al recolector para distinguirlo de los otros
 * */
int recolector(int queue, int nro){
    mensaje m;
    int result;

    while(1){
        generarBasura(&m);
        m.tipo = BASURA_RECOLECTADA;
        printf("Recolector %d: [%ld,%s]\n",nro,m.tipo,m.msg);
                
        result = msgsnd(queue, &m, BYTES_MSG,0);
        if (result == -1){
            printf("Error en la escritura mensaje: %s\n",strerror(errno));
            exit(1);
        }
        sleep(VEL_GEN_BASURA);//Intervalo de tiempo en el que no se genera basura
    }
            
    return 0;
}

/*  Funcion que contiene el codigo que realiza el clasificador
 *  colaRec es el identificador de la cola entre Recolectores y Clasificadores
 *  colaSend es el identificador de la cola entre Clasificadores y Recicladores
 *  nro es el numero asignado al clasificador para identificarlo 
 *
 * */
int clasificador(int colaRec, int colaSend, int nro){
    mensaje m;
    int result;

    while(1){
        //Recibimos basura de recolectores
        result = msgrcv(colaRec, &m, BYTES_MSG, 0, 0);
        if(result==-1){
            printf("Error al recibir mensaje: %s\n",strerror(errno));
        }
                
        printf("Clasificador %d Recibe: [%ld,%s]\n",nro,m.tipo,m.msg);
                
        //La clasificamos
        if(strcmp(m.msg,"vidrio")==0){
            m.tipo=VIDRIO;
        }else if(strcmp(m.msg,"carton")==0){
            m.tipo=CARTON;
        }else if(strcmp(m.msg,"plastico")==0){
            m.tipo=PLASTICO;
        }else if(strcmp(m.msg,"aluminio")==0){
            m.tipo=ALUMINIO;
        }else{
            printf("Error: Basura no reconocida\n");
        }
                
        sleep(VEL_CLASIFICACION_BASURA);//Intervalo de tiempo que tarda en clasificar
        
        //Se envia a la cola de mensaje hacia recicladores
        printf("Clasificador %d envia: [%ld,%s]\n",nro,m.tipo,m.msg);
        result = msgsnd(colaSend, &m, BYTES_MSG, 0);
        if(result==-1){
            printf("Error al enviar a reciclador: %s\n",strerror(errno));
        }
                
    }
    
    return 0;
}

/*Funcion que contiene el codigo del reciclador
 * colaRec es el identificador de la cola entre Clasificadores y Recicladores
 * tipoBasura es el numero que identifica al tipo de basura que el Reciclador puede recibir
 * strBasura es la cadena de caracteres del tipo de basura ("vidrio","carton","plastico","aluminio")
 * 
 * */
int reciclador(int colaRec, int tipoBasura, char* strBasura){
    mensaje m;
    int ayudar;
    
    while(1){
        //Primero revisa si puede recibir basura del tipo que le corresponde
        ayudar = msgrcv(colaRec, &m, BYTES_MSG,tipoBasura,IPC_NOWAIT);
        if(ayudar>0){
            sleep(VEL_RECICLADO_BASURA);
            printf("Rec. %s: [%ld,%s]\n",strBasura,m.tipo,m.msg);
        }else if (ayudar ==-1 & errno==ENOMSG){//Si hay error, no encontro mensaje de su tipo entonces...
            ayudar = msgrcv(colaRec, &m, BYTES_MSG,0,IPC_NOWAIT);//intenta ayudar otro
            if(ayudar>0){
                if(m.tipo==tipoBasura){
                    printf("Rec. %s: [%ld,%s]\n",strBasura,m.tipo,m.msg);//Se "ayuda" a si mismo
                }else{
                    printf("Rec. %s: Ayude reciclador de %s con [%ld,%s]\n",strBasura,m.msg,m.tipo,m.msg);//Ayuda a otro
                }
            }else if(ayudar ==-1 & errno==ENOMSG){//Sino se pone a tomar mate
                printf("Rec. %s: Nadie para ayudar. Tomando unos mates...\n",strBasura);                    
                sleep(VEL_TOMANDO_MATE);
            }else{
                printf("%s\n",strerror(errno));                    
            }
        }
    }
}



//PLANTA DE RECICLADO CON COLA DE MENSAJES
int main(){

    int pid, colaMsgRaC, colaMsgCaVCPA;
    int cola, cola1, cola2, result;
    mensaje m;
    key_t key_rac=1241, key_car=456;
    
    //Creamos Colas de Mensajes
    //Cola desde Recolectores a clasificadores
    colaMsgRaC = msgget(key_rac,IPC_CREAT|0666);
    if(colaMsgRaC<0){
        printf("Error al crear cola de Mensajes de Recolectores a Clasificadores\n");
        printf("%s\n",strerror(errno));
    }
    
    //Cola desde Clasificadores a Recolectores
    colaMsgCaVCPA = msgget(key_car, IPC_CREAT|0666);
    if(colaMsgCaVCPA<0){
        printf("Error al crear cola de Mensajes de Clasificadores a Recolectores\n");
        printf("%s\n",strerror(errno));
    }
    
    
    //Creamos recolectores
    for(int i=0; i<CANT_REC; i++){
        pid = fork();
        
        if(pid<0){
            printf("Error al crear recolector\n");
            
        }else if(pid==0){
            cola = msgget(key_rac, 0666);
            if(cola<0){
                printf("Error al obtener id de cola: %s\n",strerror(errno));
            }
			recolector(cola,i); //Codigo del recolector
            exit(1);
        }
        
        sleep(VEL_CREACION_RECOLECTORES);//Intervalo de tiempo entre la creacion de uno y otro para que rnd()
                                         //genere basuras distintas
    }
    
    //Creamos Clasificadores
    for(int i=0; i<CANT_CLAS; i++){
        pid = fork();
        
        if(pid<0){
            printf("Error al crear Clasificador: %s\n",strerror(errno));
            
        }else if(pid==0){
            //Vinculamos a las colas
            cola1 = msgget(key_rac,0666);//Cola de recolectores a clasificadores
            if(cola1<0){
                printf("Error en clasificador: %s\n",strerror(errno));
            }
            cola2 = msgget(key_car,0666);//Cola de clasificadores a recicladores
            if(cola2<0){
                printf("Error en clasificador: %s\n",strerror(errno));
            }
            clasificador(cola1, cola2, i);
            exit(1);
        }
    }  
    
    
    
    //--Creamos recicladores--//
    //Reciclador de vidrio
    pid = fork();
    if(pid<0){
        printf("Error al crear reciclador de vidrio\n");
        printf("%s\n",strerror(errno));
        
    }else if(pid==0){
        cola = msgget(key_car,0666);
        if(cola<0){
            printf("Error al asociarse a cola desde Reciclador: %s\n",strerror(errno));
        }
        
        reciclador(cola, VIDRIO, "vidrio");
        exit(1);
    }  
    
    //Reciclador de carton
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de carton\n");
        
    }else if(pid==0){
        cola = msgget(key_car,0666);
        if(cola<0){
            printf("Error al asociarse a cola desde Reciclador: %s\n",strerror(errno));
        }
		
        reciclador(cola, CARTON, "carton");
        exit(1);
    }
    
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de plastico\n");
        
    }else if(pid==0){
        cola = msgget(key_car,0666);
        if(cola<0){
            printf("Error al asociarse a cola desde Reciclador: %s\n",strerror(errno));
        }
        
		reciclador(cola, PLASTICO, "plastico");
        exit(1);
    }
    
    //Reciclador de aluminio
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de aluminio\n");
    }else if(pid==0){
        cola = msgget(key_car,0666);
        if(cola<0){
            printf("Error al asociarse a cola desde Reciclador: %s\n",strerror(errno));
        }
        
		reciclador(cola, ALUMINIO, "aluminio");
        exit(1);
    }
    
    //El padre espera por todos los procesos creados
    for(int i=0; i<7; i++){
        wait(NULL);
    }
    
    return 0;
}
