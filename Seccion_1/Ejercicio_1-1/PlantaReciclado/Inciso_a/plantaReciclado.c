#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>

#include "macros.h"

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
    int pid, ayuda;
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
    
    //Cambiamos flag de pipes para que sean no bloqueantes por el Read End
    fcntl(vidrio[RE],F_SETFL,O_NONBLOCK);
    fcntl(carton[RE],F_SETFL,O_NONBLOCK);
    fcntl(plastico[RE],F_SETFL,O_NONBLOCK);
    fcntl(aluminio[RE],F_SETFL,O_NONBLOCK);
    
    //Creamos Reclectores
    for(int i=0; i<CANT_REC; i++){
        pid = fork();
        if(pid<0){
            printf("Error al crear recoector\n");
        }else if(pid==0){
        
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
		close(carton[WE]);
		close(plastico[WE]);
		close(aluminio[WE]);
        
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			ayuda = read(vidrio[RE],&b,sizeof(basura));
            
            if(ayuda>=0){//Encontro basura en su pipe
                printf("Rec. vidrio: %s\n",b.tipo);
                
            }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de carton
                ayuda = read(carton[RE],&b,sizeof(basura));
                if(ayuda>=0){
                    printf("Rec. vidrio ayuda a Rec. cartón: %s\n",b.tipo);
                    
                }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de plastico
                    ayuda = read(plastico[RE],&b,sizeof(basura));
                    if(ayuda>=0){
                        printf("Rec. vidrio ayuda a Rec. plastico: %s\n",b.tipo);
                        
                    }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de aluminio
                        ayuda = read(aluminio[RE],&b,sizeof(basura));
                        if(ayuda>=0){
                            printf("Rec. vidrio ayuda a Rec. aluminio: %s\n",b.tipo);
                            
                        }else if(ayuda==-1 && errno==EAGAIN){//Si no pudo ayudar a nadie, toma mate
                            printf("Rec. vidrio se toma unos mates...\n");
                            sleep(VEL_TOMANDO_MATE);
                        }
                    }
                }
            }
            sleep(VEL_RECICLADO_BASURA);
        }
        
        return 0;
    }
    
    //Reciclador de carton
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de carton\n");
    }else if(pid==0){
        close(carton[WE]);
		close(vidrio[WE]);
		close(plastico[WE]);
		close(aluminio[WE]);
        
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			ayuda = read(carton[RE],&b,sizeof(basura));
            
            if(ayuda>=0){//Encontro basura en su pipe
                printf("Rec. carton: %s\n",b.tipo);
                
            }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de vidrio
                ayuda = read(vidrio[RE],&b,sizeof(basura));
                if(ayuda>=0){
                    printf("Rec. carton ayuda a Rec. vidrio: %s\n",b.tipo);
                    
                }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de plastico
                    ayuda = read(plastico[RE],&b,sizeof(basura));
                    if(ayuda>=0){
                        printf("Rec. carton ayuda a Rec. plastico: %s\n",b.tipo);
                        
                    }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de aluminio
                        ayuda = read(aluminio[RE],&b,sizeof(basura));
                        if(ayuda>=0){
                            printf("Rec. carton ayuda a Rec. aluminio: %s\n",b.tipo);
                            
                        }else if(ayuda==-1 && errno==EAGAIN){//Si no pudo ayudar a nadie, toma mate
                            printf("Rec. carton se toma unos mates...\n");
                            sleep(VEL_TOMANDO_MATE);
                        }
                    }
                }
            }
            sleep(VEL_RECICLADO_BASURA);
		}
        return 0;
    }
    
    //Reciclador de plastico
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de plastico\n");
    }else if(pid==0){
        close(plastico[WE]);
		close(carton[WE]);
		close(vidrio[WE]);
		close(aluminio[WE]);
        
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			ayuda = read(plastico[RE],&b,sizeof(basura));
            
            if(ayuda>=0){//Encontro basura en su pipe
                printf("Rec. plastico: %s\n",b.tipo);
                
            }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de carton
                ayuda = read(carton[RE],&b,sizeof(basura));
                if(ayuda>=0){
                    printf("Rec. plastico ayuda a Rec. cartón: %s\n",b.tipo);
                    
                }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de plastico
                    ayuda = read(vidrio[RE],&b,sizeof(basura));
                    if(ayuda>=0){
                        printf("Rec. plastico ayuda a Rec. vidrio: %s\n",b.tipo);
                        
                    }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de aluminio
                        ayuda = read(aluminio[RE],&b,sizeof(basura));
                        if(ayuda>=0){
                            printf("Rec. plastico ayuda a Rec. aluminio: %s\n",b.tipo);
                            
                        }else if(ayuda==-1 && errno==EAGAIN){//Si no pudo ayudar a nadie, toma mate
                            printf("Rec. plastico se toma unos mates...\n");
                            sleep(VEL_TOMANDO_MATE);
                        }
                    }
                }
            }
            sleep(VEL_RECICLADO_BASURA);
		}
        return 0;
    }
    
    //Reciclador de aluminio
    pid=fork();
    if(pid<0){
        printf("Error al crear reciclador de aluminio\n");
    }else if(pid==0){
        close(aluminio[WE]);
		close(carton[WE]);
		close(plastico[WE]);
		close(vidrio[WE]);
        
		close(RaC[RE]);
		close(RaC[WE]);		
		
		while(1){
			ayuda = read(aluminio[RE],&b,sizeof(basura));
            
            if(ayuda>=0){//Encontro basura en su pipe
                printf("Rec. aluminio: %s\n",b.tipo);
                
            }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de carton
                ayuda = read(carton[RE],&b,sizeof(basura));
                if(ayuda>=0){
                    printf("Rec. aluminio ayuda a Rec. cartón: %s\n",b.tipo);
                    
                }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de plastico
                    ayuda = read(vidrio[RE],&b,sizeof(basura));
                    if(ayuda>=0){
                        printf("Rec. aluminio ayuda a Rec. vidrio: %s\n",b.tipo);
                        
                    }else if(ayuda==-1 && errno==EAGAIN){//Intenta ayudar al de aluminio
                        ayuda = read(plastico[RE],&b,sizeof(basura));
                        if(ayuda>=0){
                            printf("Rec. aluminio ayuda a Rec. plastico: %s\n",b.tipo);
                            
                        }else if(ayuda==-1 && errno==EAGAIN){//Si no pudo ayudar a nadie, toma mate
                            printf("Rec. aluminio se toma unos mates...\n");
                            sleep(VEL_TOMANDO_MATE);
                        }
                    }
                }
            }
            sleep(VEL_RECICLADO_BASURA);
		}
        return 0;
    }
    
    close(RaC[RE]);
    close(RaC[WE]);
    close(vidrio[RE]);
    close(vidrio[WE]);
    close(carton[RE]);
    close(carton[WE]);
    close(plastico[RE]);
    close(plastico[WE]);
    close(aluminio[RE]);
    close(aluminio[WE]);
    
    //El padre espera por todos los procesos creados
    for(int i=0; i<8; i++){
        wait(NULL);
    }

    return 0;
}
