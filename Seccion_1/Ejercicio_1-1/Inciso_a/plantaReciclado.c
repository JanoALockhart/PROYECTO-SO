#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#include "../utils/macros.h"
#include "../utils/genBasura.h"

#define RE 0 //READ-END pipe
#define WE 1 //WRITE-END pipe

/* Funcion: (void *) generarBasura(basura *b)
 * Entrada:
 *      -b: puntero a una struct de tipo basura.
 * Salida: ---
 * 
 * Genera aleatoriamente un nombre de basura para asignarselo
 * al campo tipo de b.
 * Las basuras pueden ser: vidrio, carton, plastico, aluminio
 *         
 */
void* generarBasura(basura *b){
    srand(time(NULL));
	int num = rand();
    
	if(num%4==0){
        strcpy(b->tipo,"vidrio");
	}else if(num%4==1){
        strcpy(b->tipo,"carton");
    }else if(num%4==2){
        strcpy(b->tipo,"plastico");
    }else if(num%4==3){
        strcpy(b->tipo,"aluminio");
    }
}


int main(){
    //Declaracion variables
    int RaC[2], vidrio[2], carton[2], plastico[2], aluminio[2];
    int pid;
    basura b;
    
    //Creamos pipes
    if(pipe(RaC)<0){
        printf("Error al crear pipe Rac \n");
    }

    if(pipe(vidrio)<0){
        printf("Error al crear pipe vidrio  \n");
    }
    
    if(pipe(carton)<0){
        printf("Error al crear pipe carton\n");
    }
    
    if(pipe(plastico)<0){
        printf("Error al crear pipe plastico\n");
    }
    
    if(pipe(aluminio)<0){
        printf("Error al crear pipe aluminio\n");
    }
    
    //Creamos Reclectores
    for(int i=0; i<CANT_REC; i++){
        pid = fork();
        if(pid<0){
            printf("Error al crear recoector\n");
        }else if(pid==0){
        
            srand(time(NULL));
            close(RaC[RE]);
			
			close(vidrio[RE]);
			close(vidrio[WE]);
			close(carton[RE]);
			close(carton[WE]);
			close(plastico[RE]);
			close(plastico[WE]);
			close(aluminio[RE]);
			close(aluminio[WE]);
            
            while(1){
                generarBasura(&b);
                printf("Recolector %d: %s\n",i,b.tipo);
                write(RaC[WE],&b,sizeof(basura));
                sleep(VEL_GEN_BASURA);//Intervalo de tiempo en el que no se genera basura
            }
            return 0;
        }
        sleep(VEL_CREACION_RECOLECTORES);//Para asegurar que generen aleatoriamente cosas distintas
    }
    
    //Creamos Clasificadores
    for(int i=0; i<CANT_CLAS; i++){
        pid = fork();
        if(pid<0){
            printf("Error al crear Clasificador\n");
        }else if(pid==0){
            //Cerramos pipes
            close(RaC[WE]);
            close(vidrio[RE]);
            close(carton[RE]);
            close(plastico[RE]);
            close(aluminio[RE]);
            
            while(1){
                read(RaC[RE], &b, sizeof(basura));
                printf("Clasif %d: %s\n",i,b.tipo);
                
                if(strcmp(b.tipo,"vidrio")==0){
                    write(vidrio[WE],&b,sizeof(basura));
                }else if(strcmp(b.tipo,"plastico")==0){
                    write(plastico[WE],&b,sizeof(basura));
                }else if(strcmp(b.tipo,"carton")==0){
                    write(carton[WE],&b,sizeof(basura));
                }else if(strcmp(b.tipo,"aluminio")==0){
                    write(aluminio[WE],&b,sizeof(basura));
                }else{
                    printf("basura no identificada\n");
                }
                
                sleep(VEL_CLASIFICACION_BASURA);//Intervalo de tiempo que tarda en clasificar
            }
            
            return 0;
        }
        
    }
    
    //Creamos recicladores
    //Reciclador de vidrio
    pid = fork();
    if(pid<0){
        printf("Error al crear reciclador de vidrio\n");
    }else if(pid==0){
        
        close(vidrio[WE]);
		
		close(carton[RE]);
		close(carton[WE]);
		close(plastico[RE]);
		close(plastico[WE]);
		close(aluminio[RE]);
		close(aluminio[WE]);
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			read(vidrio[RE],&b,sizeof(basura));
            sleep(VEL_RECICLADO_BASURA);
			printf("Rec. vidrio: %s\n",b.tipo);
		}
        
        return 0;
    }
    
    //Reciclador de carton
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de carton\n");
    }else if(pid==0){
        close(carton[WE]);
		
		close(vidrio[RE]);
		close(vidrio[WE]);
		close(plastico[RE]);
		close(plastico[WE]);
		close(aluminio[RE]);
		close(aluminio[WE]);
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			read(carton[RE],&b,sizeof(basura));
            sleep(VEL_RECICLADO_BASURA);
			printf("Rec. Carton: %s\n",b.tipo);
		}
        return 0;
    }
    
    //Reciclador de plastico
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de plastico\n");
    }else if(pid==0){
        close(plastico[WE]);
		
		close(carton[RE]);
		close(carton[WE]);
		close(vidrio[RE]);
		close(vidrio[WE]);
		close(aluminio[RE]);
		close(aluminio[WE]);
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			read(plastico[RE],&b,sizeof(basura));
            sleep(VEL_RECICLADO_BASURA);
			printf("Reciclador Plastico: %s\n",b.tipo);
		}
        return 0;
    }
    
    //Reciclador de aluminio
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de aluminio\n");
    }else if(pid==0){
        close(aluminio[WE]);
		
		close(carton[RE]);
		close(carton[WE]);
		close(plastico[RE]);
		close(plastico[WE]);
		close(vidrio[RE]);
		close(vidrio[WE]);
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			read(aluminio[RE],&b,sizeof(basura));
            sleep(VEL_RECICLADO_BASURA);
			printf("Reciclador. alum: %s\n",b.tipo);
		}
        return 0;
    }
    
    //El padre espera por todos los procesos creados
    for(int i=0; i<8; i++){
        wait(NULL);
    }

    return 0;
}
