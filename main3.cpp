//librerias C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
//librerias cpp
#include <iostream>
#include <string>
#include <vector>
//definicion de clases

int cantidadDeDatosPorBridge(int n);
void iniciar(int cantidadMensajes);
void instrucciones();

int main(int argc, char *argv[]) {
    int opt= 0;
    /* 0 = false
    1 = true */
    int on = 0;
    int help = 0;
    int cantidadDeMensajes;
    char *nombre, *numero;
    //struct para las opciones que existen
    static struct option long_options[] = {
        {"on",     required_argument,  0,  'o' },
        {"help",    no_argument,		0,  'h' },
        {0,         0,                  0,   0  }
    };

    int long_index =0;
    //captura de las posibles respuestas
    while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
        switch (opt) {
             case 'o' : 
                    on = 1;
                    numero = optarg;
                    cantidadDeMensajes = atoi(numero);
                    break;
             case 'h' :
                    help = 1;
                    break;
             default: 
                    instrucciones(); 
                    exit(EXIT_FAILURE);
                    break;
        }
    }
    if (nombre == NULL ) {
        instrucciones();
        exit(EXIT_FAILURE);
    }

    if (on == 1) {
        if(cantidadDeMensajes==0)
            printf("no se puede inicializar con 0 mensajes, por favor elija otra cantidad\n");
        else
            printf("Se desea enviar: %i mensajes\n", cantidadDeMensajes);
            iniciar(cantidadDeMensajes);
    }
    if (help == 1){
    	instrucciones();
        exit(EXIT_FAILURE);
    }
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

void instrucciones() {
    printf("Para utilizar el programa, es necesario escribir: \n./nombrePrograma.o --on cantidadDeMensajes\n");
}

void iniciar(int cantidadDeMensajes){

    int laCantidadDeDatosPorBridge=cantidadDeDatosPorBridge(cantidadDeMensajes);
    printf("laCantidadDeDatosPorBridge: %d\n", laCantidadDeDatosPorBridge);

    LAN red1,red2,red3,red4,red5;
    Bridge B1 (1,&red1,&red2,&red5);
    Bridge B2 (2,&red3,&red4,&red5);
    B1.start();
    B2.start();
    //red1
    Equipo a(1,&red1);
    Equipo b(2,&red1);
    Equipo c(3,&red1);
    Equipo d(4,&red1);
    //red2
    Equipo e(5,&red2);
    Equipo f(6,&red2);
    Equipo g(7,&red2);
    Equipo h(8,&red2);
    //red3
    Equipo i(9,&red3);
    Equipo j(10,&red3);
    Equipo k(10,&red3);
    Equipo l(11,&red3);
    //red4
    Equipo m(12,&red4);
    Equipo n(13,&red4);
    Equipo o(15,&red4);
    Equipo p(16,&red4);

    a.start(cantidadDeMensajes);
    b.start(cantidadDeMensajes);
    c.start(cantidadDeMensajes);
    d.start(cantidadDeMensajes);
    e.start(cantidadDeMensajes);
    f.start(cantidadDeMensajes);
    g.start(cantidadDeMensajes);
    h.start(cantidadDeMensajes);
    i.start(cantidadDeMensajes);
    j.start(cantidadDeMensajes);
    k.start(cantidadDeMensajes);
    l.start(cantidadDeMensajes);
    m.start(cantidadDeMensajes);
    n.start(cantidadDeMensajes);
    o.start(cantidadDeMensajes);
    p.start(cantidadDeMensajes);

    a.wait();
    b.wait();
    c.wait();
    d.wait();
    e.wait();
    f.wait();
    g.wait();
    h.wait();
    i.wait();
    j.wait();
    k.wait();
    l.wait();
    m.wait();
    n.wait();
    o.wait();
    p.wait();
}

