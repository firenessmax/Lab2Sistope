//librerias C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//librerias cpp
#include <iostream>
#include <string>
//definicion de clases
class LAN//clase para el paso de mensajes
{
private:
	pthread_mutex_t Lmutex;//equivalente a declararlo e inicializarlo
	std::string buffer;
public:
	LAN(){pthread_mutex_init(&(this->Lmutex), NULL);}

	void loadMessage(std::string msg){this->buffer=msg;}
	std::string getMessage(){return this->buffer;}

	//bloquear LAN
	int tomarLinea(){return pthread_mutex_lock(&mylock);}
	void soltarLinea(){return pthread_mutex_lock(&mylock);}
	//estas funciones equivalen a lock y unlock pero para una LAN especifica permitiendo declarar secciones criticas de LAN
};

//prototipo de funciones
//la idea es hacer que las funciones que daban referencias cruzadas hacerlas al estilo C 




int main(int argc, char const *argv[])
{
	nuevo A,B;
	A.a=1;
	B.a=2;
	A>>&B;
	std::cout<<B.a<<std::endl;
	return 0;
}
//clases
//funciones
