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
	
	std::string buffer;
public:
	pthread_mutex_t Lmutex;//equivalente a declararlo e inicializarlo
	pthread_mutex_t Rmutex;
	pthread_mutex_t Wmutex;
	int id_receptor;
	LAN();
	void loadMessage(int id_receptor, std::string msg){
		this->id_receptor=id_receptor;this->buffer=msg;
	}
	int hayMenssage(int eq_id){return eq_id==id_receptor;}
	std::string getMessage(){id_receptor=0;return this->buffer;}
	//bloquear LAN
	int tomarLinea(){return pthread_mutex_trylock(&(this->Lmutex));}
	void soltarLinea(){pthread_mutex_unlock(&(this->Lmutex));}
	//estas funciones equivalen a lock y unlock pero para una LAN especifica permitiendo declarar secciones criticas de LAN
};
class Equipo{
	private:
		
		std::string nombre;
		int E_id;
		pthread_t thread;
	public:
		int m_recibidos;
		int m_enviados;
		LAN* l;
		Equipo(int id,LAN* l){this->E_id=id;this->l=l;} 
		void start(int cantMensajes);
		void wait(){pthread_join(this->thread, NULL);}
		void reciveMessage();
		int getID(){return this->E_id;}
};

class Bridge{
		pthread_t thread;
	public:
		int B_id;
		int m_recibidos;
		int m_enviados;
		LAN** redes;
		Bridge(int id,LAN** l){this->B_id=id;this->redes=l;} 
		void start();
		void wait(){pthread_join(this->thread, NULL);}
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

	LAN red;
	/*
	Bridge b1(3,(LAN**)red);
	b1.start();*/
	Equipo a(1,&red);
	Equipo b(2,&red);

	b.start(2);
	a.start(2);
	
	
	a.wait();
	b.wait();

	return 0;
}
LAN::LAN(){
	pthread_mutex_init(&(this->Lmutex), NULL);
	pthread_mutex_init(&(this->Rmutex), NULL);
	pthread_mutex_init(&(this->Wmutex), NULL);
}
//clases
void Equipo::start(int cantMensajes){
	this->m_recibidos=cantMensajes;
	this->m_enviados=cantMensajes;
	int rc 	= 	pthread_create(&this->thread, NULL,startEquipo, (void*) this );
	//printf("Hello World! It's me, thread with argument %d!\n", this->E_id);
}
void Bridge::start(){
	int rc 	= 	pthread_create(&this->thread, NULL,startBridge, NULL/*argumento*/ );
	//printf("Hello World! It's me, thread with argument %d!\n", this->B_id);
}

//funciones
void* startEquipo(void* arg){
	Equipo* eq= (Equipo*) arg;
	LAN* lan=eq->l;
	while(eq->m_enviados||eq->m_recibidos){

		if(eq->m_enviados&&!pthread_mutex_trylock(&(lan->Rmutex))){
			if(!pthread_mutex_trylock(&(lan->Wmutex))){
				eq->m_enviados--;
				lan->loadMessage(((eq->getID()==1)?2:1),((eq->getID()==1)?"A->":"B->"));
				printf("%cW;\n",(char)( 64+eq->getID() ) );

			}
			pthread_mutex_unlock(&(lan->Rmutex));
			continue;
		}
		if(eq->m_recibidos&&!pthread_mutex_trylock(&(lan->Rmutex))){
			
			if( lan->hayMenssage( eq->getID() ) ){
				std::string msg=lan->getMessage();
				eq->m_recibidos--;
				printf("%cR;\n",(char)( 64+eq->getID() ) );
				pthread_mutex_unlock(&(lan->Wmutex));
			}
			pthread_mutex_unlock(&(lan->Rmutex));
		}
	}
	return NULL;
}

void* startBridge(void* arg){
	printf("uninplemented function startBridge\n");
	return NULL;
}