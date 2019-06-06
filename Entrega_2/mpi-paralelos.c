#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#define MASTER 0


//Recibe un indice y solo hara las combinaciones que comiencen con ese indice
int buscarEnLista(int index, int cantElems, int tomadosDe, int suma, int* lista){
	//Chequea que existan combinaciones comenzando con ese indice
	if (index > (cantElems-tomadosDe)){
		return 0;
	}
	
	int cantSuma = 0;
	int* comb = (int*)malloc(sizeof(int)*tomadosDe);
	for(int i = 0; i < tomadosDe; i++){
		comb[i] = i+index;
	}
	while(comb[0] == index){

		//Sumar si da el num buscado
		int acum = 0;
		for(int i = 0; i < tomadosDe; i++){
			acum += lista[comb[i]];
		}
		if(acum == suma){
			cantSuma++;
			for(int i=0; i < tomadosDe; i++){
			}
		}
		
		//Pasar a la siguiente combinacion
		comb[tomadosDe-1]++;
		int indexOver = tomadosDe-1;
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
	}
	free(comb);
	return cantSuma;
}

int main(int argc, char *argv[])
{
    //Inicialización
    int cantProcesos, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &cantProcesos);
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
    MPI_Bcast(lista, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    //División de trabajo entre master y slaves.
    int elementos = 0;
    if(id == MASTER){
        int proxIndice = 0;
        int id_slave;
        while(proxIndice < N){
            MPI_Recv(&id_slave, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&proxIndice, 1, MPI_INT, id_slave, 0, MPI_COMM_WORLD);
            proxIndice++;
        }
        for(int i = 1; i < cantProcesos; i++){
            MPI_Send(&proxIndice, 1, MPI_INT, i, 1, MPI_COMM_WORLD); //TAG en 1 = terminó.
        }
    }else{
        int indice;
        MPI_Send(&id, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
        MPI_Recv(&indice, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        while(status.MPI_TAG == 0){
            elementos += buscarEnLista(indice, N, n, v, lista);
            MPI_Send(&id, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
            MPI_Recv(&indice, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }
    }
    printf("P%d\n",id);
    int resultados;
    MPI_Reduce(&elementos, &resultados, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);
    if(id == MASTER){
        printf("Soy el master en total hubo %d elementos\n", resultados);
    }

    MPI_Finalize();
}
