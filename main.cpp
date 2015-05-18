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
		int lanUsadas;
		LAN* redes[3];
		Bridge(int id,LAN* l1,LAN* l2);
		Bridge(int id,LAN* l1,LAN* l2,LAN* l3);
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

//
int indices[3]={1,1,2};

int main(int argc, char const *argv[])
{
	//<<--- inserte getopt aqui

	LAN red,red2;
	Bridge B1 (4,&red,&red2);
	B1.start();
	Equipo a(1,&red);
	Equipo b(2,&red);

	Equipo c(3,&red2);

	a.start(1);
	
	b.start(1);
	
	c.start(1);

	
	a.wait();
	b.wait();
	c.wait();

	return 0;
}

//clases
LAN::LAN(){
	this->id_receptor=0;
	pthread_mutex_init(&(this->Lmutex), NULL);
	pthread_mutex_init(&(this->Rmutex), NULL);
	pthread_mutex_init(&(this->Wmutex), NULL);
}
void Equipo::start(int cantMensajes){
	this->m_recibidos=cantMensajes;
	this->m_enviados=cantMensajes;
	int rc 	= 	pthread_create(&this->thread, NULL,startEquipo, (void*) this );
	//printf("Hello World! It's me, thread with argument %d!\n", this->E_id);
}
void Bridge::start(){
	int rc 	= 	pthread_create(&this->thread, NULL,startBridge, (void*) this  );
	//printf("Hello World! It's me, thread with argument %d!\n", this->B_id);
}
Bridge::Bridge(int id,LAN* l1,LAN* l2){
	this->B_id=id;
	this->lanUsadas=2;
	this->redes[0]=l1;
	this->redes[1]=l2;
}
Bridge::Bridge(int id,LAN* l1,LAN* l2,LAN* l3){
	this->B_id=id;
	this->lanUsadas=3;
	this->redes[0]=l1;
	this->redes[1]=l2;
	this->redes[2]=l3;
}
//funciones
void* startEquipo(void* arg){
	Equipo* eq= (Equipo*) arg;
	LAN* lan=eq->l;
	int i=0;
	while(eq->m_enviados||eq->m_recibidos){
		if(eq->m_enviados&&!pthread_mutex_trylock(&(lan->Rmutex))){
			if(!pthread_mutex_trylock(&(lan->Wmutex))){
				eq->m_enviados--;
				int id_receptor=(eq->getID()+i)%3+1;//<<-- el modulo indica el numero maximo de nodos
				std::string s="";
				s+=(char)(64+eq->getID());
				s+="->";
				lan->loadMessage(id_receptor,s);
				i++;
			}
			pthread_mutex_unlock(&(lan->Rmutex));
			continue;
		}
		if(eq->m_recibidos&&!pthread_mutex_trylock(&(lan->Rmutex))){
			
			if( lan->hayMenssage( eq->getID() ) ){
				std::string msg=lan->getMessage();
				eq->m_recibidos--;
				printf("%s%c;\n",&msg[0u],(char)( 64+eq->getID() ));
				pthread_mutex_unlock(&(lan->Wmutex));
			}
			pthread_mutex_unlock(&(lan->Rmutex));

		}
	}
	return NULL;
}

void* startBridge(void* arg){
	Bridge* b=(Bridge*) arg;
	int dest[3];
	std::string mensajes[3];//buffer message para evitar dentro de lo posible el deadlock
	//entrada1
	while(true){//revisar la condicion de termino... yo creo que parecido a los anteriores quizas contando los que corresponde y los del buffer algo asi

		if(b->redes[0]->id_receptor){
			printf("(?)->%c\n",(char)(b->redes[0]->id_receptor+64));
			if(!pthread_mutex_trylock(&(b->redes[0]->Rmutex))){
				printf("*(?)->%c\n",(char)(b->redes[0]->id_receptor+64));
				LAN* lan=b->redes[0];
				if(lan->hayMenssage(3))//deberia comprobarse para todos los que no pertenecen a la red 0
					mensajes[0]=lan->getMessage();
				pthread_mutex_unlock(&(b->redes[0]->Wmutex));
				pthread_mutex_unlock(&(b->redes[0]->Rmutex));
			}
		}
	}
	return NULL;
}