#ifndef __MACROS__MINISHELL
#define __MACROS__MINISHELL

//TAMAÑO DE LOS INPUTS
#define TAM_INPUT 512

//Nombres comandos
#define MS_EXIT "exit"
#define MS_CREAR_DIRECTORIO "createDir"
#define MS_ELIMINAR_DIRECTORIO "deleteDir"
#define MS_CREAR_ARCHIVO "createFile"
#define MS_LISTAR_CONTENIDO_DIRECTORIO "listDir"
#define MS_MOSTRAR_CONTENIDO_ARCHIVO "viewFile"
#define MS_AYUDA "help"
#define MS_MODIF_PERMISOS_ARCH "setFilePer"


//Numero de parametros en los comandos (incluyendo el nombre del comando)
#define MS_NPARAM_EXIT 1
#define MS_NPARAM_CREAT_DIR 2
#define MS_NPARAM_ELIM_DIR 2
#define MS_NPARAM_CREAT_FILE 2 
#define MS_NPARAM_LIST_DIR 2
#define MS_NPARAM_VIEW_FILE 2
#define MS_NPARAM_HELP 1
#define MS_NPARAM_MPA 11

//Nombre programa comando
#define MS_NOM_CREAT_DIR "./createDir"
#define MS_NOM_ELIM_DIR "./deleteDir"
#define MS_NOM_CREAT_FILE "./createFile"
#define MS_NOM_LIST_DIR "./listDir"
#define MS_NOM_VIEW_FILE "./viewFile"
#define MS_NOM_HELP "./help"
#define MS_NOM_MPA "./setFilePer"

//Nombre archivo con la ayuda
#define MS_HELP_TEXT "ayuda.txt"

#endif
