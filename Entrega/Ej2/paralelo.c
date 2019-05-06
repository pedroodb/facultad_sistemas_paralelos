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

double *matrices,*res;
int i,j,N,M,NUM_THREADS;
double timetick;

void* calcularMatrices(void* id) {
  int thread = *((int*)id);
  int cantMatrices = M/NUM_THREADS;
  for(int i = cantMatrices*thread; i < cantMatrices*(thread+1); i++) {
    int begin = N*N*i;
    int end = N*N*(i+1);
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
    res[i]+=(max-min)/(sum/(N*N));
  }  
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
  res=(double*)malloc(sizeof(double)*M);

  //Inicializa las matrices en 1, se supone que B y D estaran ordenadas por columnas
  for(i=0;i<N*N*M;i++){
    matrices[i]=1;
  }

  for(i=0;i<M;i++){
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



