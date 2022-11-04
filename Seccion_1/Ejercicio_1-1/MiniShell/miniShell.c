#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils/macros.h"

/*Funcion liberarEspacioArgumentos
 * char **param: Es un puntero a un arreglo que almacena argumentos
 * int argc: es la cantidad de componentes que tiene ese arreglo
 * 
 * Esta funcion libera el espacio reservado para cada uno de los argumentos
 * del arreglo y el espacio para el arreglo de punteros a los argumentos.
 * 
 * */
void liberarEspacioArgumentos(char **param, int argc){
    for(int i=0; i<argc; i++){
        free(param[i]);
    }
    free(param);
}

/*Funcion obtenerParametros
 * char *cmd: Es una cadena con argumentos separadas por espacios.
 * char ***puntArgv: Es el puntero que se devolvera. Apunta al arreglo con
 *                   todos los argumentos separados.
 * 
 * Esta funcion desglosa la cadena cmd en el comando y sus argumentos y la almacena en un arreglo. 
 * En caso de exito devuelve la cantidad de argumentos desglosados (el tamaño del arreglo)
 * 
 * */
int obtenerParametros(char *cmd, char*** puntArgv){
    int count;
    char *arg;
    char **param;
    
    count=0;
    param = malloc(sizeof(char*));
    
    //Obtener argumento, asignarle espacio, aumentar espacio para otro argumento
    arg = strtok(cmd," ");
    while(arg!=NULL){
        param[count] = malloc(strlen(arg)+1);
        strcpy(param[count],arg);
        count++;
        param = realloc(param,(count+1)*sizeof(char*));
        arg = strtok(NULL," ");
    }
    //En el ultimo cambiar \n por \0
    param[count-1][strlen(param[count-1])-1]='\0';
    param[count]=NULL;
    *puntArgv = param;
    
    return count;
}


/*Procedimiento cargarComando
 * char *cmd: es el comando completo que escribio el usuario
 * 
 * Crea un proceso hijo y se le carga el codigo del archivo de la primera cadena de cmd
 * y como argumentos las siguientes cadenas dentro de cmd
 * 
 * */
void cargarComando(char* cmd){
    int pid, cantArg;
    char **param;
    char dirCmd[TAM_INPUT] = "./";
    int nroError;
    
    pid=0;
    param=NULL;
    
    cantArg = obtenerParametros(cmd,&param);
    
    pid = fork();
    if(pid==0){//hijo
        strcat(dirCmd,param[0]);
        if(execv(dirCmd,param)==-1){
            printf("ERROR comando '%s' inexistente. Use 'help'\n",param[0]);
        } 
        exit(EXIT_FAILURE);
        
    }else if(pid<0){ //error
        printf("ERROR: No se pudo crear hijo para el comando\n");
    
    }else{//padre
        wait(&nroError);
        nroError = WEXITSTATUS(nroError);
        
        if(nroError==CANT_ARG_INCORRECTA){
            printf("La cantidad de argumentos ingresada para %s es incorrecta \n",param[0]);
        }
        
        liberarEspacioArgumentos(param,cantArg);
    }
}


int main(){
    char cmd[TAM_INPUT];
    char *primerCad;
    char **param;
    int cantArg, exit;
    
    cantArg=0;
    exit=0;
    
    printf("Bienvenido a la Mini Shell \n");
    while(!exit){
        printf("$ ");
        
        //Obtenemos la cadena que el usuario escribio
        fgets(cmd,sizeof(cmd),stdin);
        if(cmd==NULL){
            printf("ERROR al leer el comando: %s\n",strerror(errno));
        }
        
        //La primer cadena debe ser el comando
        sscanf(cmd,"%ms",&primerCad);
        
        //Detectamos el comando a ejecutar
        if(primerCad!=NULL){
            //COMANDO EXIT
            if(strcmp(primerCad,MS_EXIT)==0){
                cantArg = obtenerParametros(cmd,&param);
                if(cantArg==1){
                    printf("Hasta luego!\n");
                    exit=1;
                }else{
                    printf("La cantidad de argumentos para exit es erronea\n");
                }
                liberarEspacioArgumentos(param,cantArg);
            
            //OTRO COMANDO
            }else{
                cargarComando(cmd);
            }
        
            free(primerCad);
        }
    }
    

    return 0;
}



