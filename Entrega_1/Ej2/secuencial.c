#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

//Funcion para calcular el tiempo
double dwalltime() {
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}


int main(int argc,char*argv[]){
  double *matrices;
  int i,j,N,M;
  double timetick;

  //Controla los argumentos al programa
  if (argc <= 2){
    printf("\n Faltan argumentos\n");
    return 0;
  }
 
  N=atoi(argv[1]);
  M=atoi(argv[2]);

  //Aloca memoria para las matrices (almacena todas las matrices en un solo arreglo)
  matrices=(double*)malloc(sizeof(double)*N*N*M);

  //Inicializa las matrices en 1, el resultado deberia ser 0
  for(i=0;i<N*N*M;i++){
    matrices[i]=1;
  }

  double res=0;

  timetick = dwalltime();

  //Realiza la busqueda del maximo, minimo y promedio y va acumulando la sumatoria en res
  for(i=0;i<M;i++){
    int begin = N*N*i;
    int end = N*N*(i+1);
    int max=-1;
    int min=__INT_MAX__;
    int sum=0;
    for(j=begin;j<end;j++){
        int val=matrices[j];
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
