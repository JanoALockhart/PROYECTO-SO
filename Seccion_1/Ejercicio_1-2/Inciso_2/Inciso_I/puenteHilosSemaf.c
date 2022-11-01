#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define ERROR -1
#define TIEMPO_EN_EL_PUENTE 5
#define TIEMPO_APARECE_AUTO 1

#define BUFF_TIME 64

#define NORTE 1
#define SUR 2

//Declaracion variables de semaforos
sem_t permDir; //Asegura exclusion entre autos del Sur y Norte
sem_t entSalSur, cantSur; //mutex entre autos Sur y la cantidad en el puente
sem_t entSalNorte, cantNorte; //mutex entre autos Norte y la cantidad en el puente


//Auto del Sur
void* autoSur(){   
    //Darle una identificacion al auto segun la hora en que aparecio
    int horaf;
    
    horaf = time(NULL);
    
    printf("Auto en el SUR APARECE- %d\n",horaf);
    //Protocolo de entrada
    sem_wait(&entSalSur);
    if(sem_trywait(&cantSur)==ERROR){//Si es el primero el Sur...
        sem_wait(&permDir);
    }else{
        sem_post(&cantSur);
    }
    sem_post(&cantSur);
    printf("Auto del SUR ENTRO al puente - %d\n",horaf);
    sem_post(&entSalSur);
    
    //Seccion Critica
    printf("Auto del SUR PASANDO por el puente - %d\n",horaf);
    sleep(TIEMPO_EN_EL_PUENTE);
    
    //Protocolo de salida
    sem_wait(&entSalSur);
    sem_wait(&cantSur);
    printf("Auto del SUR SALIO del puente - %d\n",horaf);
    if(sem_trywait(&cantSur)==ERROR){//Si fue el ultimo del Sur...
        sem_post(&permDir);
    }else{
        sem_post(&cantSur);
    }
    sem_post(&entSalSur);   
    pthread_exit(NULL);
}


//Auto del Norte
void* autoNorte(){
    //Darle una identificacion al auto segun la hora en la que aparecio
    int horaf;

    horaf = time(NULL);

    printf("Auto en el NORTE APARECE- %d\n",horaf);
    //Protocolo de entrada
    sem_wait(&entSalNorte);
    if(sem_trywait(&cantNorte)==ERROR){//Si es el primero del Norte...
        sem_wait(&permDir);
    }else{
        sem_post(&cantNorte);
    }
    sem_post(&cantNorte);
    printf("Auto del NORTE ENTRO al puente - %d\n",horaf);
    sem_post(&entSalNorte);
    
    //Seccion Critica
    printf("Auto del NORTE PASANDO por el puente - %d\n",horaf);
    sleep(TIEMPO_EN_EL_PUENTE);
    
    //Protocolo de salida
    sem_wait(&entSalNorte);
    sem_wait(&cantNorte);
    printf("Auto del NORTE SALIO del puente - %d\n",horaf);
    if(sem_trywait(&cantNorte)==ERROR){//Si fue el ultimo del Norte...
        sem_post(&permDir);
    }else{
        sem_post(&cantNorte);
    }
    sem_post(&entSalNorte);
    pthread_exit(NULL);
}

//Procedimiento que genera hilos de autos de la direccion
//pasada por parametro
void* generadorAutos(void* dir){
    pthread_t hiloAuto;
    while(1){
        if(dir==(void*)(intptr_t)NORTE){
            pthread_create(&hiloAuto,NULL,autoNorte,NULL);
        }else{
            pthread_create(&hiloAuto,NULL,autoSur,NULL);
        }
        sleep(TIEMPO_APARECE_AUTO);
    }
    
    pthread_join(hiloAuto,NULL);
    
}



int main(){
    pthread_t genNorte, genSur;

    //Inicializacion semaforos
    if(sem_init(&permDir,0,1)==-1){
        printf("Error al inicializar semaforo\n");
        exit(EXIT_FAILURE);
    }
    
    if(sem_init(&entSalSur,0,1)==-1){
        printf("Error al inicializar semaforo\n");
        exit(EXIT_FAILURE);
    }
    
    if(sem_init(&entSalNorte,0,1)==-1){
        printf("Error al inicializar semaforo\n");
        exit(EXIT_FAILURE);
    }
    
    if(sem_init(&cantSur,0,0)==-1){
        printf("Error al inicializar semaforo\n");
        exit(EXIT_FAILURE);
    }
    
    if(sem_init(&cantNorte,0,0)==-1){
        printf("Error al inicializar semaforo\n");
        exit(EXIT_FAILURE);
    }
    
    //Generar autos
    pthread_create(&genNorte,NULL,generadorAutos,(void*)(intptr_t)NORTE);
    pthread_create(&genNorte,NULL,generadorAutos,(void*)(intptr_t)SUR);
    
    pthread_join(genNorte,NULL);
    pthread_join(genSur,NULL);
    
    return 0;
}
