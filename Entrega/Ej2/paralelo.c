#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Para calcular el tiempo
double dwalltime(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

double *matrices;
int i,j,N,M,NUM_THREADS;
double timetick;

void* calcularMatriz(void* id) {
  int id = *((int*)id);
  int blockSize = M/
  int cantM = 
}

int main(int argc,char*argv[]){

 //Controla los argumentos al programa
  if (argc <= 2){
   printf("\n Faltan argumentos\n");
   return 0;
  }
 
  N=atoi(argv[1]);
  M=atoi(argv[2]);

 //Aloca memoria para las matrices
  matrices=(double*)malloc(sizeof(double)*N*N*M);

 //Inicializa las matrices en 1, se supone que B y D estaran ordenadas por columnas
  for(i=0;i<N*N*M;i++){
    matrices[i]=1;
  }

  timetick = dwalltime();

 //Realiza la multiplicacion

  int res=0;

  for(i=0;i<M;i++){
    int max=-1;
    int min=__INT_MAX__;
    int sum=0;
    for(j=0;j<N*N;j++){
        int val=matrices[i*N*N+j];
        sum+=val;
        if(val>max){
            max=val;
        } else if (val<min) {
            min=val;
        }
    }
    res+=(max-min)/(sum/(N*N));
  }

  printf("Tiempo en segundos %f \n", dwalltime() - timetick);

  if(res==0){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }

 free(matrices);
 return(0);
}



/*****************************************************************/



