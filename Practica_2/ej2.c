#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int N, NUM_THREADS, NUM;
int* vector;
int* result;
int block_size;

void* buscar(void* id){
  int block = *((int*)id);
  result[block] = 0;
  for(int i = (block*block_size); i < ((1+block)*block_size); i++){
    if(vector[i] == NUM) result[block]++;
  }
  pthread_exit(0);
}

int main(int argc, char* argv[]){
  N = atoi(argv[1]);
  NUM_THREADS = atoi(argv[2]);
  NUM = atoi(argv[3]);
  block_size = N/NUM_THREADS;
  
  int ids[NUM_THREADS];  
  result = (int*)malloc(sizeof(int)*NUM_THREADS);
  vector = (int*)malloc(sizeof(int)*N);
  srand(time(NULL));

  for(int i = 0; i < N; i++){
    vector[i] = rand() % 5;
    printf(" %d", vector[i]);
  }
  printf("\n");
  pthread_attr_t attr;
  pthread_t threads[NUM_THREADS];
  pthread_attr_init(&attr);
  
  for(int i = 0; i < NUM_THREADS; i++){
    ids[i] = i;
    pthread_create(&threads[i], &attr, buscar, &ids[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }
  int res;
  for(int i = 0; i < NUM_THREADS; i++) res+= result[i];
  printf("\n Result: %d\n", res);

}
