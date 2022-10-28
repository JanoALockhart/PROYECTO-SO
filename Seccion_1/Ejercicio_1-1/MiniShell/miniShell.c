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
 * int cantParam: Es la cantidad de argumentos que recibe el comando
 * 
 * Esta funcion recibe un comando con sus argumentos en una cadena, crea un arreglo
 * donde almacena cada una de las palabras en el comando y devuelve
 * un puntero al mismo. La cantidad de palabras que se reciben
 * debe especificarse en cantParam. Si la cantidad de palabras encontradas
 * en el comando es distinta a la especificada en cantParam, devuelve -1.
 * En caso de exito devuelve 0.
 * 
 * */
int obtenerParametros(char *cmd, char*** puntArgv,int cantParam){
    int count,error;
    char *arg;
    char **param;
    
    count=0;
    error=0;
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
    *puntArgv = param;
    
    if(count!=cantParam){
        printf("La cantidad de parametros ingresada es invalida\n");
        liberarEspacioArgumentos(param,count);
        error=-1;
        *puntArgv=NULL;
    }
    
    return error;
}

int main(){
    char cmd[TAM_INPUT];
    char *primerCad;
    char **param;
    int result, exit, pid;
    
    result=0;
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
                result = obtenerParametros(cmd,&param,MS_NPARAM_EXIT);
                if(result==0){
                    printf("Hasta luego!\n");
                    exit=1;
                }
            
            //COMANDO SETFILEPER
            }else if(strcmp(primerCad,MS_MODIF_PERMISOS_ARCH)==0){
                result = obtenerParametros(cmd,&param,MS_NPARAM_MPA);
                if(result==0){
                    pid = fork();
                    if(pid==0){
                        execv("./setFilePer",param);
                    }else if(pid<0){
                        printf("ERROR: No se pudo crear hijo para el comando\n");
                    }
                    wait(NULL);
                }
            //COMANDO MOSTRAR CONTENIDO DE UN ARCHIVO
            
            
            //COMANDO AYUDA
                
            //COMANDO NO ENCONTRADO
            }else{
                printf("ERROR comando '%s' no es valido\n",primerCad);
            }
        }
        
        free(primerCad);
    }
    

    return 0;
}



