#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

/*
 * argv[0] = generadorAutos
 * argv[1] = direccion en la que iran los autos que genera (NORTE/SUR)
 * 
 * */
int main(int argc, char *argv[]){
    
    int pid;
    int nroAuto;
    char strNum[10];
    
    nroAuto=0;
    
    while(1){
        pid=fork();
        if(pid==0){    
            sprintf(strNum,"%d",nroAuto);
            
            if(strcmp(argv[1],STR_SUR)==0){
                char *argv[] = {AUTO_NAME,STR_SUR,strNum,NULL};
                if(execv(AUTO_FILE,argv)==-1){
                    printf("ERROR al cargar imagen auto SUR: %s\n",strerror(errno));
                }
                
            }else if(strcmp(argv[1],STR_NORTE)==0){
                char *argv[] = {AUTO_NAME,STR_NORTE,strNum,NULL};
                if(execv(AUTO_FILE,argv)==-1){
                    printf("ERROR al cargar imagen auto NORTE: %s\n",strerror(errno));
                }

            }else{
                printf("ERROR direccion no valida para auto\n");
            }
            exit(EXIT_FAILURE);
            
        }else if(pid<0){
            printf("ERROR al crear auto de %s: %s\n",argv[1],strerror(errno));
        }
        nroAuto++;
        sleep(TIEMPO_APARICION_AUTOS);
    }
    
    return 0;
}

