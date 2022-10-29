#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils/macros.h"

int main(int argc, char *argv[]){

    //Se llama a viewFile con el archivo que contiene el texto de la ayuda
    char *param[] = {MS_NOM_VIEW_FILE, MS_HELP_TEXT};
    if(execv(MS_NOM_VIEW_FILE,param)==-1){
        printf("ERROR al cargar la imagen del comando\n");
    }
    
    exit(EXIT_FAILURE); //Si llega aqui, no cargo la imagen de viewFile
}

