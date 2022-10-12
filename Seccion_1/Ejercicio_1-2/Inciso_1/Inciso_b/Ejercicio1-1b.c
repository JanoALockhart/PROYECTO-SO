#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define CANT_REC 3
#define CANT_CLAS 2

#define KEY_RAC (key_t)(1234);
#define KEY_CAR (key_t)(4321);

//Mensaje
typedef struct mensaje{
	long tipo;
	char msg[20];
} mensaje;

//Funcion
void* generarBasura(mensaje *m){
	
	int num = rand();
	if(num%4==0){
        strcpy(m->msg,"vidrio");
	}
	else{
		if(num%4==1){
			strcpy(m->msg,"carton");
		}else{
			if(num%4==2){
				strcpy(m->msg,"plastico");
			}
			else{
				if(num%4==3){
					strcpy(m->msg,"aluminio");
				}
			}
		}
	}
}	


int main(){

    int pid, colaMsgRaC, colaMsgCaVCPA;
    int cola;
    mensaje m;
	long longitud;
    
    //Creamos Colas de Mensajes
    //Cola desde Recolectores a clasificadores
    colaMsgRaC = msgget(KEY,IPC_CREAT);
    if(colaMsgRaC<0){
        printf("Error al crear cola de Mensajes de Recolectores a Clasificadores");
    }
    
    //Cola desde Clasificadores a Recolectores
    colaMsgCaVCPA = msgget(KEY, IPC_CREAT);
    if(colaMsgCaVCPA<0){
        printf("Error al crear cola de Mensajes de Clasificadores a Recolectores");
    }
    
    //Creamos recolectores
    for(int i=0; i<CANT_REC; i++){
        pid = fork();
        if(pid<0){
            printf("Error al crear recoector\n");
        }else if(pid==0){
            //Codigo recolector
            cola = msgget(KEY, 0666);
			m.tipo = 1;
			while(1){
				generarBasura(&m);
				printf("Recolector %d: %s\n",i,m.tipo);
				longitud = sizeof(struct mensaje)-sizeof(long);
				if (msgsnd(colaMsgRaC, &m, longitud,0) == -1){
					printf("Error en la escritura mensaje\n");
					exit(1);
				}
			}
            return 0;
        }
    }
    
    //Creamos Clasificadores
    for(int i=0; i<CANT_CLAS; i++){
        pid = fork();
        if(pid<0){
            printf("Error al crear Clasificador\n");
        }else if(pid==0){
            //Codigo del clasificador
            return 0;
        }
    }  
    
    //Creamos recicladores
    //Reciclador de vidrio
    
    pid = fork();
    if(pid<0){
        printf("Error al crear reciclador de vidrio\n");
    }else if(pid==0){
        //Codigo reciclador vidrio
        return 0;
    }  
    
    //Reciclador de carton
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de carton\n");
    }else if(pid==0){
        //Codigo reciclador de carton
        return 0;
    }
    
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de plastico\n");
    }else if(pid==0){
        //Codigo reciclador plastico
        return 0;
    }
    
    //Reciclador de aluminio
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de aluminio\n");
    }else if(pid==0){
        //Codigo reciclador aluminio
        return 0;
    }
    
    //El padre espera por todos los procesos creados
    for(int i=0; i<8; i++){
        wait(NULL);
    }
    
    return 0;
}
