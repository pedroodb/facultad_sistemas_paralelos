#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double dwalltime(){
  double sec;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

//Recibe un indice y solo hara las combinaciones que comiencen con ese indice
int cantCombIndex(int index, int cantElems, int tomadosDe, int suma, int* lista){
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

int main(int argc, char* argv[]){
	if(argc < 4){
		printf("Faltan argumentos\n");
		return 1;
	}

	int N = atoi(argv[1]);
	int tomadosDe = atoi(argv[2]);
	int suma = atoi(argv[3]);	

	int *lista = (int*) malloc(sizeof(int)*N);

	for(int i = 0; i < N; i++){
		lista[i] = i+1;
	}

	double timetick = dwalltime();

	int cant = 0;
	for (int i = 0; i < N; i++){
		cant += cantCombIndex(i,N,tomadosDe,suma,lista);
	}
	
	printf("Resultado: %d\nTiempo: %f\n", cant,dwalltime()-timetick);

	free(lista);
}
