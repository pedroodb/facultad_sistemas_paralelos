#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#define MASTER 0

double dwalltime(){
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

void nextComb(int skip, int tomadosDe, int cantElems, int* comb){
    int indexOver = tomadosDe-1; //tomadosDe-1 es el tamano de comb
    comb[tomadosDe-1]+=skip; 
    int carry = comb[indexOver] - (cantElems-tomadosDe+indexOver);
    while (carry > 0 && (comb[0] <= (cantElems-tomadosDe))){
        int over = 0;
        while (indexOver > 0 && (comb[indexOver] > (cantElems-tomadosDe+indexOver))){
            indexOver--;
            comb[indexOver]++;
            over = 1;
        }
        if(over){
            for(int i = (indexOver+1); i < tomadosDe; i++){
                comb[i] = comb[i-1]+1;
            }
        }
        carry--;
        comb[tomadosDe-1] += carry; 
        indexOver = tomadosDe-1;
        carry = comb[indexOver] - (cantElems-tomadosDe+indexOver);
    }
}

//Calculara las combinaciones que sumen suma, salteando de a skip combinaciones
int buscarEnLista(int cantElems, int tomadosDe, int suma, int skip, int* comb, int* lista){
	int cantSuma = 0;
	while(comb[0] <= (cantElems-tomadosDe)){
		//Sumar si da el num buscado
		int acum = 0;	
		for(int i = 0; i < tomadosDe; i++){
			acum += lista[comb[i]];
		}
		if(acum == suma){
			cantSuma++;
		}		
        nextComb(skip,tomadosDe,cantElems,comb);        
	}
	return cantSuma;
}

int slave(int id, int cantProc, int* lista, int cantElems, int tomadosDe, int suma){
	int* comb = (int*)malloc(sizeof(int)*tomadosDe);
	for(int i = 0; i < tomadosDe; i++){
		comb[i] = i;
	}
    nextComb(id,tomadosDe,cantElems,comb);
    int cant = buscarEnLista(cantElems,tomadosDe,suma,cantProc,comb,lista);
	free(comb);
    return cant;
}

int main(int argc, char *argv[])
{
    //Inicialización
    int cantProc, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Status status;

    int N = atoi(argv[1]);
    int n = atoi(argv[2]);
    int v = atoi(argv[3]);

    //Armado de la lista y envío a todos los procesos
    int* lista = (int *) malloc(sizeof(int)*N);
    if(id == MASTER){
        for(int i = 0; i < N; i++){
            lista[i] = i+1;
        }
    }

    double timetick = dwalltime();

    MPI_Bcast(lista, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    //División de trabajo entre master y slaves.
    int elementos = slave(id, cantProc, lista, N, n, v);
    printf("id: %d -- Tiempo: %f\n",id ,dwalltime()-timetick);
    //printf("P:%d, cant:%d\n",id,elementos);
    int resultados;
    MPI_Reduce(&elementos, &resultados, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);
    if(id == MASTER){
    	printf("Resultado: %d\nTiempo: %f\n", resultados,dwalltime()-timetick);
    }

    MPI_Finalize();
}
