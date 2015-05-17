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
	int tomarLinea(){return pthread_mutex_lock(&(this->Lmutex));}
	void soltarLinea(){pthread_mutex_lock(&(this->Lmutex));}
	//estas funciones equivalen a lock y unlock pero para una LAN especifica permitiendo declarar secciones criticas de LAN
};
class Equipo{
   	private:
   		LAN* l;
   		std::string nombre;
    		int E_id;
   		pthread_t thread;
   	public:
		Equipo(int id,LAN* l);
      	void start();
      	void wait(){pthread_join(this->thread, NULL);}
      	void reciveMessage();
      	int getID(){return this->E_id;}
};

class Bridge{
	pthread_t thread;
	public:
   	int B_id;
   	LAN** redes;
   	void start();
	void wait(){pthread_join(this->thread, NULL);}
   	Bridge(int id,LAN** l){this->B_id=id;this->redes=l;} 
};

//prototipo de funciones
//la idea es hacer que las funciones que daban referencias cruzadas hacerlas al estilo C 

//malas... editar
void* startEquipo(void* arg);
void* startBridge(void* arg);
void sendMessage( Equipo* emisor,Equipo* receptor, std::string msg);
/*
void sendMessagethroughBridge(std::string msg,IPuente* p, Equipo* receptor);
void repiteMessage(std::string msg, IEquipo* receptor);
*/
//fin de malas

//puede que las use para la implementacion de start()
int main(int argc, char const *argv[])
{
	//<<--- inserte getopt aqui
	return 0;
}
//clases
void Equipo::start(){
	int rc 	= 	pthread_create(&this->thread, NULL,startEquipo, NULL/*argumento*/ );
    printf("Hello World! It's me, thread with argument %d!\n", this->E_id);
}
void Bridge::start(){
	int rc 	= 	pthread_create(&this->thread, NULL,startBridge, NULL/*argumento*/ );
	printf("Hello World! It's me, thread with argument %d!\n", this->B_id);
}

//funciones
void* startEquipo(void* arg){
	printf("uninplemented function\n");
	return NULL;
}
void* startBridge(void* arg){
	printf("uninplemented function\n");
	return NULL;
}