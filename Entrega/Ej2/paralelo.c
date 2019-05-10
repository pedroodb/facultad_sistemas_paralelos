#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Funcion para calcular el tiempo
double dwalltime(){
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}

double *matrices,*res;
int N,M,NUM_THREADS;
double timetick;

//Funcion ejecutada por cada matriz para calcular el resultado de las matrices que le correspondan
void* calcularMatrices(void* id) {
  int thread = *((int*)id);
  int cantMatrices = M/NUM_THREADS;
  for(int i = cantMatrices*thread; i < cantMatrices*(thread+1); i++) {
    int begin = N*N*i;
    int end = N*N*(i+1);
    int max=-1;
    int min=__INT_MAX__;
    int sum=0;
    for(int j=begin;j<end;j++){
        int val=matrices[j];
        sum+=val;
        if(val>max){
          max=val;
        } else if (val<min) {
          min=val;
        }
    }
    res[thread]+=(max-min)/(sum/(N*N));
  }  
}

int main(int argc,char*argv[]){

 //Controla los argumentos al programa
  if (argc <= 3){
   printf("\n Faltan argumentos\n");
   return 0;
  }
 
  N=atoi(argv[1]);
  M=atoi(argv[2]);
  NUM_THREADS=atoi(argv[3]);

  //Aloca memoria para las matrices (almacena todas las matrices en un solo arreglo)
  matrices=(double*)malloc(sizeof(double)*N*N*M);
  res=(double*)malloc(sizeof(double)*NUM_THREADS);

  //Inicializa las matrices en 1, el resultado deberia ser 0
  for(int i=0;i<N*N*M;i++){
    matrices[i]=1;
  }

  for(int i=0;i<NUM_THREADS;i++){
    res[i]=0;  
  }

  int ids[NUM_THREADS];  

  pthread_t threads[NUM_THREADS];

  timetick = dwalltime();

  //Lanza los threads para que realicen el calculo
  for(int i = 0; i < NUM_THREADS; i++){
    ids[i] = i;
    pthread_create(&threads[i], NULL, calcularMatrices, &ids[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  //Acumula el resultado de la ejecucion de los threads
  double resultado=0;
  for(int i = 0; i < NUM_THREADS; i++){
    resultado+=res[i];
  }
  

  printf("Tiempo en segundos %f \n", dwalltime() - timetick);

  if(resultado==0){
    printf("Multiplicacion de matrices resultado correcto\n");
  }else{
    printf("Multiplicacion de matrices resultado erroneo\n");
  }

  free(matrices);
  free(res);
  return(0);
}
