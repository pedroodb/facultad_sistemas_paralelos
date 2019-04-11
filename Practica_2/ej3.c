#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Para calcular tiempo
double dwalltime(){
  double sec;
  struct timeval tv;

  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

int N, NUM_THREADS;
int* vector;
int* result;
int block_size;

void* sum(void* id){
  int block = *((int*)id);
  result[block] = 0;
  int begin = (block*block_size);
  int end = ((1+block)*block_size);
  for(int i = begin; i < end; i++){
    result[block]+=vector[i];
  }
  pthread_exit(0);
}

int main(int argc, char* argv[]){

  //Controla los argumentos al programa
  if ((argc < 3) || ((N = atoi(argv[2])) <= 0) ) {
    printf("Error en cantidad de parametros");
    exit(1);
  }

  srand(time(NULL));

  double timetick;
  NUM_THREADS = atoi(argv[1]);
  block_size = N/NUM_THREADS;
  
  int ids[NUM_THREADS];  
  result = (int*)malloc(sizeof(int)*NUM_THREADS);
  vector = (int*)malloc(sizeof(int)*N);

  for(int i = 0; i < N; i++){
    vector[i] = rand();
  }

  pthread_t threads[NUM_THREADS];
  
  timetick = dwalltime();

  //Lanza los threads para que realicen el calculo

  for(int i = 0; i < NUM_THREADS; i++){
    ids[i] = i;
    pthread_create(&threads[i], NULL, sum, &ids[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  //Calcula el resultado final sumando lo que calcularon los threads y dividiendo por N

  float res;
  for(int i = 0; i < NUM_THREADS; i++) res+= result[i];
  res = res/N;

  float time = dwalltime() - timetick;

  //Chequeo de verbose
  if ((argc == 4) && atoi(argv[3]) == 1) {
    for(int i = 0; i < N; i++){
      printf("%d, ",vector[i]);
    }
    printf("\n");
    printf("Avg: %f\n", res);
    printf("Time: ");
  }

  free(vector);
  free(result);

  printf("%f\n", time);

}
