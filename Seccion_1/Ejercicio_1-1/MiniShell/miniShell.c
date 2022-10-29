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
 * Esta funcion desglosa la cadena cmd en el comando y sus argumentos y la almacena en un arreglo. 
 * La cantidad de argumentos que recibe el comando
 * debe especificarse en cantParam. Si la cantidad de palabras encontradas
 * en el comando es distinta a la especificada en cantParam, devuelve -1 y param es NULL.
 * En caso de exito devuelve 0 y param apunta al arreglo con los argumentos.
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
        liberarEspacioArgumentos(param,count);
        error=-1;
        *puntArgv=NULL;
    }
    
    return error;
}


/*Procedimiento cargarComando
 * char *cmd: es el comando completo que recibio el usuario
 * int cantArg: es la cantidad de argumentos que recibe el archivo del comando
 * char *fileCmd: es el nombre del archivo en el que esta implementado el comando
 * 
 * Crea un proceso hijo y se le carga el codigo del archivo fileCmd con los argumentos de cmd.
 * 
 * */
void cargarComando(char* cmd, int cantArg, char *fileCmd){
    int pid, result;
    char **param;
    
    pid=0;
    param=NULL;
    
    result = obtenerParametros(cmd,&param,cantArg);
    if(result==0){
        pid = fork();
        if(pid==0){
            if(execv(fileCmd,param)==-1){
                printf("ERROR al cargar la imagen del comando\n");
            } 
            exit(1);
        }else if(pid<0){
            printf("ERROR: No se pudo crear hijo para el comando\n");
        }
        wait(NULL);
        liberarEspacioArgumentos(param,MS_NPARAM_CREAT_DIR);
    }else{
        printf("La cantidad de parametros ingresada es invalida\n");
    }
}


int main(){
    char cmd[TAM_INPUT];
    char *primerCad;
    char **param;
    int result, exit;
    
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
                liberarEspacioArgumentos(param,MS_NPARAM_EXIT);
            }
            
            //COMANDO CREAR UN DIRECTORIO
            else if(strcmp(primerCad,MS_CREAR_DIRECTORIO)==0){
                cargarComando(cmd,MS_NPARAM_CREAT_DIR,MS_NOM_CREAT_DIR);
            }
                
            //COMANDO ELIMINAR UN DIRECTORIO 
            else if(strcmp(primerCad,MS_ELIMINAR_DIRECTORIO)==0){
                cargarComando(cmd,MS_NPARAM_ELIM_DIR,MS_NOM_ELIM_DIR);
            }   
            
            //COMANDO CREAR UN ARCHIVO
            else if(strcmp(primerCad,MS_CREAR_ARCHIVO)==0){
                cargarComando(cmd,MS_NPARAM_CREAT_FILE,MS_NOM_CREAT_FILE);
            }
            
            //COMANDO LISTAR CONTENIDO DE UN DIRECTORIO
            else if(strcmp(primerCad,MS_LISTAR_CONTENIDO_DIRECTORIO)==0){
                cargarComando(cmd,MS_NPARAM_LIST_DIR, MS_NOM_LIST_DIR);
            }
            
            //COMANDO MOSTRAR CONTENIDO DE UN ARCHIVO
            else if(strcmp(primerCad,MS_MOSTRAR_CONTENIDO_ARCHIVO)==0){
                cargarComando(cmd,MS_NPARAM_VIEW_FILE, MS_NOM_VIEW_FILE);
            }
            //COMANDO AYUDA
            else if(strcmp(primerCad,MS_AYUDA)==0){
                cargarComando(cmd,MS_NPARAM_HELP,MS_NOM_HELP);
            }
            
            //COMANDO MODIFICAR PERMISOS DE UN ARCHIVO (MPA)
            else if(strcmp(primerCad,MS_MODIF_PERMISOS_ARCH)==0){
                cargarComando(cmd,MS_NPARAM_MPA,MS_NOM_MPA);
            }
            //COMANDO NO ENCONTRADO
            else{
                printf("ERROR comando '%s' no existe. Use 'help'\n",primerCad);
            }
        }
        
        free(primerCad);
    }
    

    return 0;
}



