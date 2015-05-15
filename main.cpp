//librerias C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//librerias cpp
#include <iostream>
#include <string>
#include <typeinfo>
//defClases
//clase para el medio de comunicacion
class LAN
{
  private:
	std::string buffer;
  public:
	void loadMessage(std::string msg){this->buffer=msg;}
	std::string getMessage(){return this->buffer;}
};
//clase base para todos los nodos
class INodo{
	//cada nodo se comporta como un thread por lo que debe llevar la informacion de un nodo
	int thread_id;
	pthread_t this_thread;
	//metodos para funcionar como nodo;
	virtual void start();//solo prototipo la idea es que sea implementado
	void wait(){pthread_join(this->this_thread, NULL);}
	virtual void sendMessage(std::string msg, INodo* receptor);
	virtual void reciveMessage();//no estoy seguro de si declararlo en la interfaz
};
class Bridge:public INodo{
	void sendMessagethroughBridge(std::string msg,Bridge p, INodo* receptor);
};
class Equipo:public INodo{
  protected:
	LAN* l;
	std::string nombre;
	int thread_id;
	pthread_t this_thread;
  public:
	Equipo(int id,LAN* l);
	static void* onStart(void* arg);//implementar funcion para enviar a la cantidad de nodos que se le indiquen, recordar que 
	void start();
	void sendMessage(std::string msg, Equipo* receptor);
	void sendMessagethroughBridge(std::string msg,Bridge p, INodo* receptor);
	void reciveMessage();
};

//|||variables Globales
//variable global adyacencia
int adyacencia[3][3]={{0,0,1},
					  {0,0,2},
					  {1,2,0}};


int main(int argc, char const *argv[])
{
	std::cout<<"holi!!"<<std::endl;
	return 0;
}