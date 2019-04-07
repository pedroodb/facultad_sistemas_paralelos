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
int* min;
int* max;
int block_size;

void* getMaxMin(void* id){
  int block = *((int*)id);
  max[block] = -1;
  min[block] = 10;
  for(int i = (block*block_size); i < ((1+block)*block_size); i++){
    if(vector[i] < min[block]) min[block] = vector[i];
    if(vector[i] > max[block]) max[block] = vector[i];
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
  min = (int*)malloc(sizeof(int)*NUM_THREADS);
  max = (int*)malloc(sizeof(int)*NUM_THREADS);
  vector = (int*)malloc(sizeof(int)*N);

  for(int i = 0; i < N; i++){
    vector[i] = rand() % 10;
  }

  pthread_attr_t attr;
  pthread_t threads[NUM_THREADS];
  pthread_attr_init(&attr);
  
  timetick = dwalltime();

  //Lanza los threads para que realicen el calculo

  for(int i = 0; i < NUM_THREADS; i++){
    ids[i] = i;
    pthread_create(&threads[i], &attr, getMaxMin, &ids[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  //Calcula el resultado final sumando lo que calcularon los threads y dividiendo por N

  int maximum = -1;
  int mininum = 10;
  for(int i = 0; i < NUM_THREADS; i++) {
    if(max[i]>maximum) maximum = max[i];
    if(min[i]<mininum) mininum = min[i];
  }
  
  float time = dwalltime() - timetick;

  //Chequeo de verbose
  if ((argc == 4) && atoi(argv[3]) == 1) {
    for(int i = 0; i < N; i++){
      printf("%d, ",vector[i]);
    }
    printf("\n");
    printf("Max: %d\n", maximum);
    printf("Min: %d\n", mininum);
    printf("Time: ");
  }

  printf("%f\n", time);

}