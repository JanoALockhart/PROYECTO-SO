#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "utils/macros.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    
    if(argc!=MS_NPARAM_CREAT_DIR){
        exit(CANT_ARG_INCORRECTA);
    }

    ret = mkdir(argv[1], 0755);

    if (ret != 0){
        printf("ERROR al crear el directorio '%s': %s\n",argv[1],strerror(errno));
        exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
    return 0;
}
