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
  double *A,*B,*C ,*D,*AT,*tot;
  int i,j,k,N;
  int check=1;
  double timetick;

  //Controla los argumentos al programa
  if (argc < 2){
    printf("\n Falta un argumento:: N dimension de la matriz \n");
    return 0;
  }
 
  N=atoi(argv[1]);

  //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);
  D=(double*)malloc(sizeof(double)*N*N);
  AT=(double*)malloc(sizeof(double)*N*N);
  tot=(double*)malloc(sizeof(double)*N*N);

  //Inicializa las matrices en 1, se supone que B y D estaran ordenadas por columnas y el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
  	  A[i*N+j]=1;
  	  B[j*N+i]=1;
  	  C[i*N+j]=1;
  	  D[j*N+i]=1;
    }
  }

  timetick = dwalltime();

  //Transpone la matriz A
  for(j=0;j<N;j++){
    for(k=0;k<N;k++){
      AT[j*N+k]=A[k*N+j];
    }
  }

  //Realiza las respectivas multiplicaciones y acumula en tot
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      int sum=0;
      for(k=0;k<N;k++){
    	  int pos1 = i*N+k;
    	  int pos2 = j*N+k;
    	  sum = sum + A[pos1]*AT[pos2];
      }
      tot[i*N+j]=sum;
    }
  }

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      int sum=0;
      for(k=0;k<N;k++){
        int pos1 = i*N+k;
        int pos2 = j*N+k;
        sum = sum + A[pos1]*B[pos2];
      }
      tot[i*N+j]=sum+tot[i*N+j];
    }
  }

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      int sum=0;
      for(k=0;k<N;k++){
        int pos1 = i*N+k;
        int pos2 = j*N+k;
        sum = sum + C[pos1]*D[pos2];
      }
    tot[i*N+j]=sum+tot[i*N+j];
    }
  }

  printf("Tiempo en segundos %f \n", dwalltime() - timetick);

  //Verifica el resultado
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
	    check=check&&(tot[i*N+j]==N*3);
    }
  }

  if(check){
    printf("Multiplicacion de matrices resultado correcto\n");
  }else{
    printf("Multiplicacion de matrices resultado erroneo\n");
  }

  free(A);
  free(B);
  free(C);
  free(D);
  free(tot);
  return(0);
}
