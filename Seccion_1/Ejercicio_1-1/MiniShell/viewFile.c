#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){
    
    FILE *fileDes;
    char caracter;
    
    //Abrir archivo
    fileDes = fopen(argv[1],"r");
    if(fileDes==NULL){
        printf("ERROR al abrir el archivo '%s': %s\n",argv[1],strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    //Imprimir contenido del archivo
    while(!feof(fileDes)){
        caracter = fgetc(fileDes);
        if(caracter!=EOF){
            printf("%c",caracter);
        }
    }
    
    //Cerrar archivo
    if(fclose(fileDes)!=0){
        printf("ERROR al cerrar el archivo '%s': %s\n",argv[1],strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
    return 0;
}
