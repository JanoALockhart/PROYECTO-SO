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
                printf("%s\n",strerror(errno));
            }
			while(1){
				generarBasura(&m);
                m.tipo = BASURA_RECOLECTADA;
				printf("Recolector %d: [%ld,%s]\n",i,m.tipo,m.msg);
                
                result = msgsnd(cola, &m, BYTES_MSG,0);
				if (result == -1){
					printf("Error en la escritura mensaje\n");
                    printf("%s\n",strerror(errno));
                    exit(1);
				}
                sleep(VEL_GEN_BASURA);//Intervalo de tiempo en el que no se genera basura
			}
            
            return 0;
        }
        
        sleep(VEL_CREACION_RECOLECTORES);//Intervalo de tiempo entre la creacion de uno y otro para que rnd()
                                         //genere basuras distintas
    }
    
    //Creamos Clasificadores
    for(int i=0; i<CANT_CLAS; i++){
        pid = fork();
        
        if(pid<0){
            printf("Error al crear Clasificador\n");
            printf("%s\n",strerror(errno));
            
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
            
            while(1){
                //Recibimos basura de recolectores
                result = msgrcv(cola1, &m, BYTES_MSG, 0, 0);
                if(result==-1){
                    printf("%s\n",strerror(errno));
                }
                printf("Clasificador %d Recibe: [%ld,%s]\n",i,m.tipo,m.msg);
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
                    printf("Basura no reconocida\n");
                }
                
                sleep(VEL_CLASIFICACION_BASURA);//Intervalo de tiempo que tarda en clasificar
                
                //Se envia a la cola de mensaje hacia recicladores
                printf("Clasificador %d Envia: [%ld,%s]\n",i,m.tipo,m.msg);
                result = msgsnd(cola2, &m, BYTES_MSG, 0);
                if(result==-1){
                    printf("Error al enviar a reciclador\n");
                    printf("%s\n",strerror(errno));
                }
                
            }
            
            return 0;
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
            printf("Error al asociarse a cola desde Reciclador\n");
            printf("%s\n",strerror(errno));
        }
        
		while(1){
            int ayudar;
            ayudar = msgrcv(cola, &m, BYTES_MSG,VIDRIO,IPC_NOWAIT);
            if(ayudar>0){
                sleep(VEL_RECICLADO_BASURA);
			    printf("Rec. vidrio: [%ld,%s]\n",m.tipo,m.msg);
            }else if (ayudar ==-1 & errno==ENOMSG){//Si hay error, no encontro mensaje de su tipo
                ayudar = msgrcv(cola, &m, BYTES_MSG,-5,IPC_NOWAIT);//intenta ayudar otro
                if(ayudar>0){
                    if(m.tipo==VIDRIO){
                        printf("Rec. vidrio: [%ld,%s]\n",m.tipo,m.msg);
                    }else{
                        printf("Rec. Vidrio: Ayude reciclador de %s\n",m.msg);
                    }
                }else if(ayudar ==-1 & errno==ENOMSG){//Se pone a tomar mate
                    printf("Rec. Vidrio: Nadie para ayudar. Tomando unos mates...\n");                    
                    sleep(5);
                }else{
                    printf("%s\n",strerror(errno));                    
                }
			}
		}
        return 0;
    }  
    
    //Reciclador de carton
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de carton\n");
    }else if(pid==0){
        cola = msgget(key_car,0666);
		while(1){
            int ayudar;
            ayudar = msgrcv(cola, &m, BYTES_MSG,CARTON,IPC_NOWAIT);
            if(ayudar>0){
                sleep(VEL_RECICLADO_BASURA);
			    printf("Rec. vidrio: [%ld,%s]\n",m.tipo,m.msg);
            }else if (ayudar ==-1 & errno==ENOMSG){//Si hay error, no encontro mensaje de su tipo
                ayudar = msgrcv(cola, &m, BYTES_MSG,-5,IPC_NOWAIT);//intenta ayudar otro
                if(ayudar>0){
                    if(m.tipo==2){
                        printf("Rec. vidrio: [%ld,%s]\n",m.tipo,m.msg);
                    }else{
                        printf("Rec. Vidrio: Ayude reciclador de %s\n",m.msg);
                    }
                }else if(ayudar ==-1 & errno==ENOMSG){//Se pone a tomar mate
                    printf("Rec. Vidrio: Nadie para ayudar. Tomando unos mates...\n");                    
                    sleep(5);
                }else{
                    printf("%s\n",strerror(errno));                    
                }
			}
		}
        return 0;
    }
    
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de plastico\n");
    }else if(pid==0){
    
        cola = msgget(key_car,0666);
        
		while(1){
			if (msgrcv(cola, &m, BYTES_MSG,PLASTICO,0) == -1){
				printf("Error en la lectura mensaje\n");
			}
            sleep(VEL_RECICLADO_BASURA);
			printf("Rec. plástico: [%ld,%s]\n",m.tipo,m.msg);
		}
        return 0;
    }
    
    //Reciclador de aluminio
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de aluminio\n");
    }else if(pid==0){
        cola = msgget(key_car,0666);
		while(1){
			if (msgrcv(cola, &m, BYTES_MSG,ALUMINIO,0) == -1){
				printf("Error en la lectura mensaje\n");
			}
            sleep(VEL_RECICLADO_BASURA);
			printf("Rec. aluminio: [%ld,%s]\n",m.tipo,m.msg);
		}
        return 0;
    }
    
    //El padre espera por todos los procesos creados
    for(int i=0; i<7; i++){
        wait(NULL);
    }
    
    return 0;
}
