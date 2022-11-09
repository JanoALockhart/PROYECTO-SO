#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//VARIABLES SEMAFOROS
sem_t DoEA;
sem_t FA;
sem_t ABC;
sem_t CB;
sem_t BC;
sem_t BCDoE;
sem_t DoEF;

#define TIEMPO_SLEEP 0

//RUTINAS
static void* rutinaA(void* nada){
	while(1){
		sem_wait(&DoEA);
		sem_wait(&FA);

		printf("A\n");
		sleep(TIEMPO_SLEEP);
		
		sem_post(&ABC);
	}
	pthread_exit(NULL);
}

static void* rutinaB(void* nada){
	while(1){
		sem_wait(&CB);
		sem_wait(&ABC);
		
		printf("B\n");
		sleep(TIEMPO_SLEEP);
		
		sem_post(&BC);
		sem_post(&BCDoE);
	}
	pthread_exit(NULL);
}

static void* rutinaC(void* nada){
	while(1){
		sem_wait(&BC);
		sem_wait(&ABC);
		
		printf("C\n");
		sleep(TIEMPO_SLEEP);
		
		sem_post(&CB);
		sem_post(&BCDoE);
	}
	pthread_exit(NULL);
}

static void* rutinaD(void* nada){
	while(1){
		sem_wait(&BCDoE);
		
		printf("(D)\n");
		sleep(TIEMPO_SLEEP);
		
		sem_post(&DoEF);
		sem_post(&DoEA);
	}
	pthread_exit(NULL);
}

static void* rutinaE(void* nada){
	while(1){
		sem_wait(&BCDoE);
		
		printf("(E)\n");
		sleep(TIEMPO_SLEEP);
		
		sem_post(&DoEF);
		sem_post(&DoEA);
	}
	pthread_exit(NULL);
}

static void* rutinaF(void* nada){
	while(1){
		sem_wait(&DoEF);
		sem_wait(&DoEF);
		
		printf("F\n\n");
		sleep(TIEMPO_SLEEP);
		
		sem_post(&FA);
		sem_post(&FA);
	}
	pthread_exit(NULL);
}


int main(){
	int error;
	pthread_t hilos[6];
	
	error = sem_init(&DoEA,0,1);
	printf("%d",error);
	
	error = sem_init(&FA,0,2);
	printf("%d",error);
	
	error = sem_init(&CB,0,1);
	printf("%d",error);
	
	error = sem_init(&ABC,0,0);
	printf("%d",error);
	
	error = sem_init(&BC,0,0);
	printf("%d",error);
	
	error = sem_init(&BCDoE,0,0);
	printf("%d",error);
	
	error = sem_init(&DoEF,0,0);
	printf("%d \n",error);
	
	pthread_create(&hilos[0],NULL,rutinaA,NULL);
	pthread_create(&hilos[1],NULL,rutinaB,NULL);
	pthread_create(&hilos[2],NULL,rutinaC,NULL);
	pthread_create(&hilos[3],NULL,rutinaD,NULL);
	pthread_create(&hilos[4],NULL,rutinaE,NULL);
	pthread_create(&hilos[5],NULL,rutinaF,NULL);
	
	//Esperar por los hilos
	for(int i=0; i<6; i++){
		pthread_join(hilos[i],NULL);
	}
	
	return 0;
}
