#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "utils/macros.h"

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
  
    if(argc!=MS_NPARAM_LIST_DIR){
        exit(CANT_ARG_INCORRECTA);
    }
  
    d = opendir(argv[1]);
    if (d!=NULL) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }else{
        printf("ERROR al listar directorio %s: %s \n",argv[1],strerror(errno));
    }
    
    return 0;
}
