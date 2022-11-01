#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"


int main(int argc, char *argv[]){
    
    int pid;
    
    while(1){
        pid=fork();
        if(pid==0){    
        
            if(strcmp(argv[1],STR_SUR)==0){
                char *argv[] = {AUTO_NAME,STR_SUR,NULL};
                if(execv(AUTO_FILE,argv)==-1){
                    printf("ERROR al cargar imagen auto SUR: %s\n",strerror(errno));
                }
                
            }else if(strcmp(argv[1],STR_NORTE)==0){
                char *argv[] = {AUTO_NAME,STR_NORTE,NULL};
                if(execv(AUTO_FILE,argv)==-1){
                    printf("ERROR al cargar imagen auto SUR: %s\n",strerror(errno));
                }

            }else{
                printf("ERROR direccion no valida para auto\n");
            }
            exit(EXIT_FAILURE);
            
        }else if(pid<0){
            printf("ERROR al crear auto de %s: %s\n",argv[0],strerror(errno));
        }
        sleep(TIEMPO_APARICION_AUTOS);
    }
    
    return 0;
}

