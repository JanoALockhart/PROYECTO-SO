#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

#define RE 0 //READ-END pipe
#define WE 1 //WRITE-END pipe

int DoEA[2];
int FA[2];
int ABC[2];
int CB[2];
int BC[2];
int BCDoE[2];
int DoEF[2];

void A(){
	int aux;
	close(DoEA[WE]);
	close(FA[WE]);
	close(ABC[RE]);
	
	close(CB[RE]);
	close(CB[WE]);
	close(BC[RE]);
	close(BC[WE]);
	close(BCDoE[RE]);
	close(BCDoE[WE]);
	close(DoEF[RE]);
	close(DoEF[WE]);
	
	while(1){
		read(DoEA[RE],&aux,sizeof(aux));
		read(FA[RE],&aux,sizeof(aux));
		printf("A\n");
		aux=1;
		write(ABC[WE],&aux,sizeof(aux));
	}
}

void B(){
	int aux;
	close(CB[WE]);
	close(ABC[WE]);
	close(BC[RE]);
	close(BCDoE[RE]);
	
	close(DoEA[RE]);
	close(DoEA[WE]);
	close(FA[RE]);
	close(FA[WE]);
	close(DoEF[RE]);
	close(DoEF[WE]);
	
	while(1){
		read(CB[RE],&aux,sizeof(aux));
		read(ABC[RE],&aux,sizeof(aux));
		printf("B\n");
		aux=1;
		write(BC[WE],&aux,sizeof(aux));
		write(BCDoE[WE],&aux,sizeof(aux));	
	}
}

void C(){
	int aux;
	close(BC[WE]);
	close(ABC[WE]);
	close(CB[RE]);
	close(BCDoE[RE]);
	
	close(DoEA[RE]);
	close(DoEA[WE]);
	close(FA[RE]);
	close(FA[WE]);
	close(DoEF[RE]);
	close(DoEF[WE]);
	
	while(1){
		read(BC[RE],&aux,sizeof(aux));
		read(ABC[RE],&aux,sizeof(aux));
		printf("C\n");
		aux=1;
		write(CB[WE],&aux,sizeof(aux));
		write(BCDoE[WE],&aux,sizeof(aux));	
	}
}

void D(){
	int aux;
	close(BCDoE[WE]);
	close(DoEF[RE]);
	close(DoEA[RE]);
	
	close(ABC[RE]);
	close(ABC[WE]);
	close(FA[RE]);
	close(FA[WE]);
	close(CB[RE]);
	close(CB[WE]);
	close(BC[RE]);
	close(BC[WE]);
	
	while(1){
		read(BCDoE[RE],&aux,sizeof(aux));
		printf("(D)\n");
		aux=1;
		write(DoEF[WE],&aux,sizeof(aux));
		write(DoEA[WE],&aux,sizeof(aux));	
	}
}

void E(){
	int aux;
	close(BCDoE[WE]);
	close(DoEF[RE]);
	close(DoEA[RE]);
	
	close(ABC[RE]);
	close(ABC[WE]);
	close(FA[RE]);
	close(FA[WE]);
	close(CB[RE]);
	close(CB[WE]);
	close(BC[RE]);
	close(BC[WE]);
	
	while(1){
		read(BCDoE[RE],&aux,sizeof(aux));
		printf("(E)\n");
		aux=1;
		write(DoEF[WE],&aux,sizeof(aux));
		write(DoEA[WE],&aux,sizeof(aux));	
	}
}

void F(){
	int aux;
	close(DoEF[WE]);
	close(FA[RE]);
	
	close(DoEA[RE]);
	close(DoEA[WE]);
	close(ABC[RE]);
	close(ABC[WE]);
	close(CB[RE]);
	close(CB[WE]);
	close(BC[RE]);
	close(BC[WE]);
	close(BCDoE[RE]);
	close(BCDoE[WE]);
	
	while(1){
		read(DoEF[RE],&aux,sizeof(aux));
		read(DoEF[RE],&aux,sizeof(aux));
		printf("F\n\n");
		aux=1;
		write(FA[WE],&aux,sizeof(aux));
		write(FA[WE],&aux,sizeof(aux));	
	}
}

int main(){
	int pid;
	int aux=1;
	
	if(pipe(DoEA)<0){
        printf("Error al crear pipe DoEA \n");
    }
    
    if(pipe(FA)<0){
        printf("Error al crear pipe FA \n");
    }
    
    if(pipe(ABC)<0){
        printf("Error al crear pipe ABC \n");
    }
    
    if(pipe(CB)<0){
        printf("Error al crear pipe CB \n");
    }
    
    if(pipe(BC)<0){
        printf("Error al crear pipe BC \n");
    }
    
    if(pipe(BCDoE)<0){
        printf("Error al crear pipe BCDoE \n");
    }
    
    if(pipe(DoEF)<0){
        printf("Error al crear pipe DoEF \n");
    }
    
    write(DoEA[WE],&aux,sizeof(aux));
    write(FA[WE],&aux,sizeof(aux));
    write(FA[WE],&aux,sizeof(aux));
    write(CB[WE],&aux,sizeof(aux));
    
    pid=fork();
    if(pid==0){
		A();
		exit(0);
	}
	
	pid=fork();
    if(pid==0){
		B();
		exit(0);
	}
	
	pid=fork();
    if(pid==0){
		C();
		exit(0);
	}
	
	pid=fork();
    if(pid==0){
		D();
		exit(0);
	}
	
	pid=fork();
    if(pid==0){
		E();
		exit(0);
	}
	
	pid=fork();
    if(pid==0){
		F();
		exit(0);
	}
	
	if(pid!=0){
		for(int i=0; i<7;i++){
			wait(NULL);
		}
	}
}
