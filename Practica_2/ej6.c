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
int* sorted;
int* final_sorted;
int block_size;

void merge(int begin, int end, int shift) {
  printf("%d , %d, r:%d \n",begin,end,shift);
  int pos1, pos2 = 0;
  for (int i = begin; i<=end; i++) {
    if ((sorted[begin+pos1] < sorted[begin+shift+pos2] && pos1<shift) || begin+shift+pos2>end) {
      final_sorted[i] = sorted[begin+pos1];
      pos1++;
    } else {
      final_sorted[i] = sorted[begin+shift+pos2];
      pos2++;
    }
  }
  for(int i = 0; i < N; i++){
    printf("%d, ",final_sorted[i]);
  }
  printf("\n");
}

void mergeSort(int begin, int end) {
  int range = end-begin; 
  if (range<=1) {
    sorted[begin] = (vector[begin] < vector[end]) ? vector[begin] : vector[end];
    sorted[end] = (vector[begin] > vector[end]) ? vector[begin] : vector[end];
  } else {
    mergeSort(begin,begin+(range/2));
    mergeSort(begin+(range/2)+1,end);
    merge(begin,end,(range/2)+1);
  }
}

void* threadMergeSort(void* id) {
  int block = *((int*)id);
  mergeSort(block*block_size,(block*block_size)+block_size-1);
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
  sorted = (int*)malloc(sizeof(int)*N);
  vector = (int*)malloc(sizeof(int)*N);
  final_sorted = (int*)malloc(sizeof(int)*N);

  for(int i = 0; i < N; i++){
    vector[i] = rand() % 10;
  }

  pthread_attr_t attr;
  pthread_t threads[NUM_THREADS];
  pthread_attr_init(&attr);
  
  timetick = dwalltime();

  //Lanza los threads para que ordenen su respectivo bloque

  for(int i = 0; i < NUM_THREADS; i++){
    ids[i] = i;
    pthread_create(&threads[i], &attr, threadMergeSort, &ids[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  float time = dwalltime() - timetick;

  //Chequeo de verbose
  if ((argc == 4) && atoi(argv[3]) == 1) {
    for(int i = 0; i < N; i++){
      printf("%d, ",vector[i]);
    }
    printf("\n");
    for(int i = 0; i < N; i++){
      printf("%d, ",sorted[i]);
    }
    printf("\n");
    for(int i = 0; i < N; i++){
      printf("%d, ",final_sorted[i]);
    }
    printf("\n");
    printf("Time: ");
  }

  printf("%f\n", time);

}