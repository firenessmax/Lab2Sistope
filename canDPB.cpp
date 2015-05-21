#include <stdio.h>
#include <stdlib.h>

int cantidadDeDatosPorBridge(int n);

int main(int argc, char const *argv[]){
	int n = 19;
	printf("n: %i\n",n);
	int resultado = cantidadDeDatosPorBridge(n);
	printf("cantidad: %i\n",resultado);
	return 0;
}

int cantidadDeDatosPorBridge(int n){
	int contador=0;
	int aux1, aux2, aux3;
	aux1=n;
	if(aux1%15==0)
		aux1=15;
	else
		aux1=aux1%15;
	aux2=aux1;
	if(aux2>12)
		aux2=12;
	else
		aux2=aux2;
	contador+=aux2;
	for (int i = 1; i < 4; i++){
		aux2=(aux1-i);
		if(aux2<0){
			aux2=0;
		} else {
			if(aux2>12)
				aux2=12;
			else
				aux2=aux2;
		}
		contador+=aux2;
	}
	aux3=(n-1)/15;
	contador=contador+(48*aux3);

	return 4*contador;
}