#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int ret = 0;

    ret = mkdir(argv[1], 0755);

    if (ret != 0){
        printf("ERROR al crear el directorio '%s': %s\n",argv[1],strerror(errno));
        exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
    return 0;
}
