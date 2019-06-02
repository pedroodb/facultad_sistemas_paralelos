#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]){
	
	int cant, id;

	MPI_Init(&argc, &argv);

	MPI_Status estadoRcv;
	MPI_Request req;

	int s,r;

	MPI_Comm_size(MPI_COMM_WORLD, &cant); 

	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	s = id;

	MPI_Isend(&s, 1, MPI_INT, (id+1)%cant, 1, MPI_COMM_WORLD, &req);

	MPI_Recv(&r, 1, MPI_INT, (id-1+cant)%cant, 1, MPI_COMM_WORLD, &estadoRcv);

	printf("Soy %d de %d, y me llego el id %d \n", id, cant, r);

	MPI_Finalize();
}
