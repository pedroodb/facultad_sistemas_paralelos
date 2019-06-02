#include <stdio.h>
#include <stdlib.h>

int cantComb(int cantElems, int tomadosDe, int suma, int* lista){
	int cantSuma = 0;
	int* comb = (int*)malloc(sizeof(int)*tomadosDe);
	for(int i = 0; i < tomadosDe; i++){
		comb[i] = i;
	}
	while(comb[0] <= (cantElems-tomadosDe)){

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
	int n = atoi(argv[2]);
	int v = atoi(argv[3]);	

	int *lista = (int*) malloc(sizeof(int)*N);
	for(int i = 0; i < N; i++){
		lista[i] = i+1;
	}

	int c = cantComb(N,n,v,lista);
	printf("Resultado: %d\n",c);

	free(lista);
}
