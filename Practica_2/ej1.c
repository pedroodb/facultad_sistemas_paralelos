#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Dimension por defecto de las matrices
int N=100;
int NUM_THREADS = 1;
int block_size;
double *A,*B,*C;

//Para calcular tiempo
double dwalltime(){
  double sec;
  struct timeval tv;

  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

void* multiplicar(void* id){
  int block = *((int*)id);
  int start = block*block_size;
  int end = start+block_size;
  int i,j,k;
  for(i=start;i<end;i++){
    for(j=0;j<N;j++){
      int aux = 0;
      for(k=0;k<N;k++){
        aux += A[i*N+k]*B[k+j*N];
      }
      C[i*N+j] = aux;
    }
  }
  pthread_exit(0);
}

int main(int argc,char*argv[]){
  int check=1;
  double timetick;

  //Controla los argumentos al programa
  if ((argc != 3) || ((N = atoi(argv[2])) <= 0) )
  {
    printf("Error en cantidad de parametros");
    exit(1);
  }
  NUM_THREADS = atoi(argv[1]);
  int ids[NUM_THREADS];
  block_size = N/NUM_THREADS;
  int i, j;

  //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);

  //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      A[i*N+j] = 1; 
      B[i*N+j] = 1; 
    }
  }   

  // Inicializacion de threads
  pthread_attr_t attr;
  pthread_t threads[NUM_THREADS];
  pthread_attr_init(&attr);

  //Realiza la multiplicacion

  timetick = dwalltime();

  for(i=0;i<NUM_THREADS;i++){
    ids[i]= i;
    pthread_create(&threads[i], &attr, multiplicar, &ids[i]);
  }   

  for(i=0;i<NUM_THREADS;i++){
    pthread_join(threads[i], NULL);
  }

  float time = dwalltime() - timetick;

  //Verifica el resultado
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
    check=check&&(C[i*N+j]==N);
    }
  }   

  free(A);
  free(B);
  free(C);
  
  if(check){
    printf("%f\n", time);
    return(0);
  }else{
    exit(2);
  }
}
