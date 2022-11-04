#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils/macros.h"

int main(int argc, char *argv[]){

    if(argc!=MS_NPARAM_HELP){
        exit(CANT_ARG_INCORRECTA);
    }

    //Se llama a viewFile con el archivo que contiene el texto de la ayuda
    char *param[] = {MS_NOM_VIEW_FILE, MS_HELP_TEXT,NULL};
    if(execv(MS_NOM_VIEW_FILE,param)==-1){
        printf("ERROR al cargar la imagen del comando para leer ayuda\n");
    }
    
    exit(EXIT_FAILURE); //Si llega aqui, no cargo la imagen de viewFile
}

