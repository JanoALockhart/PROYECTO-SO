#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define UR 2
#define UW 3
#define UX 4
#define GR 5
#define GW 6
#define GX 7
#define OR 8
#define OW 9
#define OX 10

int main(int argc, char* argv[]){
    int error;
    mode_t newMode;
    
    newMode=0;
    error=0;
    
    /*
    printf("argc: %d\n",argc);
    for(int i=0;i<argc;i++){
        printf("arg %d: %s\n",i,argv[i]);
    }*/
    
    //Verificamos que los permisos que tiene son 0 o 1
    for(int i=2;i<argc && !error;i++){
        if(!(strcmp(argv[i],"0")==0 || strcmp(argv[i],"1")==0)){
            printf("ERROR los argumentos para los permisos solo pueden ser 0 o 1\n");
            exit(EXIT_FAILURE);
        }
    }
    
    
    //Permisos de owner
    if(strcmp(argv[UR],"1")==0){
        newMode+=S_IRUSR;
    }
    
    if(strcmp(argv[UW],"1")==0){
        newMode+=S_IWUSR;
    }
    
    if(strcmp(argv[UX],"1")==0){
        newMode+=S_IXUSR;
    }
    
    //Permisos de grupo
    if(strcmp(argv[GR],"1")==0){
        newMode+=S_IRGRP;
    }
    
    if(strcmp(argv[GW],"1")==0){
        newMode+=S_IWGRP;
    }
    
    if(strcmp(argv[GX],"1")==0){
        newMode+=S_IXGRP;
    }
    
    //Permisos de otros
    if(strcmp(argv[OR],"1")==0){
        newMode+=S_IROTH;
    }
    
    if(strcmp(argv[OW],"1")==0){
        newMode+=S_IWOTH;
    }
    
    if(strcmp(argv[OX],"1")==0){
        newMode+=S_IXOTH;
    }
    
    //Cambiamos los permisos
    if(chmod(argv[1],newMode)==-1){
        printf("Error al cambiar permisos: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
    return 0;
}
