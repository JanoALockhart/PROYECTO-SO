#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
    
    FILE *fileDes;
    char caracter;
    
    //Abrir archivo
    fileDes = fopen(argv[1],"w+");
    if(fileDes==NULL){
        printf("ERROR al abrir el archivo '%s': %s\n",argv[1],strerror(errno));
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
