#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

#include "utils/macros.h"

int main(int argc, char *argv[]){
    
    FILE *fileDes;
    char caracter;
    
    if(argc!=MS_NPARAM_CREAT_FILE){
        exit(CANT_ARG_INCORRECTA);
    }
    
    //Abrir archivo
    fileDes = fopen(argv[1],"w+");
    if(fileDes==NULL){
        printf("ERROR al abrir el archivo '%s': %s\n",argv[1],strerror(errno));
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
