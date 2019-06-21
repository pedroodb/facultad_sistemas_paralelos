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

double *matrices;
int N,M,NUM_THREADS;
double timetick;

//Funcion ejecutada por cada proceso para calcular el resultado de las matrices que le correspondan
void* calcularMatrices(void* id) {
  int thread = *((int*)id);
  int cantMatrices = M/NUM_THREADS;
  //Para cada matriz calcula el resultado y la multiplica por el valor deseado
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
    double res = (max-min)/(sum/(N*N));
    //Multiplica la matriz por ese valor
    for(int j=begin;j<end;j++){
        matrices[j]*=res;
    }
  }
  
  int primerMatriz = (cantMatrices*thread)*N*N;
  //Sumatoria de las matrices guardada en la primer matriz asignada al thread
  for (int i=(cantMatrices*thread)+1; i<cantMatrices*(thread+1);i++){
    for(int j=0;j<N*N;j++){
        matrices[primerMatriz+j]+=matrices[j+(N*N*i)];
    }
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

  //Inicializa las matrices en 1, el resultado deberia ser 0
  for(int i=0;i<N*N*M;i++){
    matrices[i]=1;
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

  //Acumula el resultado de la ejecucion de los threads en la primer matriz
  int matricesPorThread = M/NUM_THREADS;
  for(int i = 1; i < NUM_THREADS; i++){
    for(int j = 0; j < N*N; j++){
        matrices[j]+=matrices[j+(N*N*i*matricesPorThread)];
    }
  }

  int check = 0;
  for(int i=0;i<N*N;i++){
      check += matrices[i];
  }

  if (check==0){
    printf("Resultado correcto\n");
  } else {
    printf("Resultado incorrecto\n");
  }
  
  printf("Tiempo en segundos %f \n", dwalltime() - timetick);

  free(matrices);
  return(0);
}
