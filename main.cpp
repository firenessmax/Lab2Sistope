//librerias C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
//librerias cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
//definicion de clases

    
typedef struct m
{
	std::string buffer;
	std::string bufferPuente;
	int id_receptor;
	int id_lan;
} Mensaje;


class LAN//clase para el paso de mensajes
{
private:
	//buffer mensaje
	Mensaje M_buffer;
public:
	//semaforos de escritura y lectura
	pthread_mutex_t Rmutex;
	pthread_mutex_t Wmutex;
	//variables para identificacion
	int id_receptor;
	int L_id;
	//constructor
	LAN(int id);
	void loadMessage(Mensaje M_buffer){
		this->M_buffer=M_buffer;
	}
	//metodo para saber si el equipo que consulta es el receptor del mensaje en la lan
	int hayMenssage(int eq_id){return eq_id==this->M_buffer.id_receptor;}
	//metodo para obtener el mensaje
	std::string getMessage(){
		M_buffer.id_receptor=0;
		return this->M_buffer.buffer;
	}
	int getIdLan(){
		return this->M_buffer.id_lan;
	}
	int getIdReceptor(){
		int r=M_buffer.id_receptor;
		return r;
	}
	//metodo que obtiene los puentes por los que a pasado
	std::string getPuentes(){
		return M_buffer.bufferPuente;
	}
};
class Equipo{
	private:
		//identificacion del equipo
		int E_id;
		//hilo que controla el equipo
		pthread_t thread;
	public:
		//contador inverso de cantidad de enviados y recibidos
		int m_recibidos;
		int m_enviados;
		//red a la cual está conectado
		LAN* l;
		//constructor
		Equipo(int id,LAN* l){
			this->E_id=id;
			this->l=l;
		} 
		//prototipos de metodos para trabajo conthread
		void start(int cantMensajes);
		void wait(){pthread_join(this->thread, NULL);}
		//getter
		int getID(){return this->E_id;}
};

class Bridge{
	private:
		//hilo controlador
		pthread_t thread;
		//cola de mensajes para evitar el deadlock
		std::vector<Mensaje> cola;
	public:
		//informacion del puente
		int B_id;
		//contadores inversos
		int m_para_enviar;
		int m_enviados;
		//variable para distintas implementaciones
		int lanUsadas;
		//direccion de a cual red debe enviar los mensajes que toma
		int dir[16];
		//redes a las cuales puede estar conectado
		LAN* redes[3];
		//constructores
		Bridge(int id,LAN* l1,LAN* l2);
		Bridge(int id,LAN* l1,LAN* l2,LAN* l3,int dirs[]);
		//metodos para el manejo de la cola
		void encolarMensaje(Mensaje m){this->cola.push_back(m);}
		Mensaje tomarMensaje(){
			Mensaje m=this->cola.front();
			this->cola.erase(this->cola.begin());
			return m;
		}
		void devolverMensaje(Mensaje m){this->cola.insert(this->cola.begin(),m);}
		//comprobador de si la cola no está vacia
		int hayMensajes(){return this->cola.size()>0;}
		//prototipos de metodos para trabajo conthread
		void start();
		void wait(){pthread_join(this->thread, NULL);}
};

//prototipo de funciones
	//funciones para los thread
void* startEquipo(void* arg);
void* startBridge(void* arg);
	//funciones llamadas por getopt
void iniciar(int cantidadMensajes);
void instrucciones();

//variables globales 
FILE * archivo;
pthread_mutex_t Omutex=PTHREAD_MUTEX_INITIALIZER;
std::string ordenDeTermino="El orden en que cada equipo terminó de mandar mensajes fue: ";
	//indica la cantidad de equipos en ejecucion;
int enEjecucion=16;

int main(int argc, char *argv[]){ 
	int opt= 0;
    /* 0 = false
    1 = true */
    int on = 0;
    int help = 0;
    int cantidadDeMensajes;
    char *numero;
    //struct para las opciones que existen
    static struct option long_options[] = {
        {"on",     required_argument,	0,  'o' },
        {"help",    no_argument,		0,  'h' },
        {0,         0,                 	0,   0  }
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
    printf("on : %d\n",opt);
   

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

//clases
	//constructor de LAN
LAN::LAN(int id){
	this->M_buffer.id_receptor=0;
	this->M_buffer.id_lan=0;
	this->id_receptor=0;
	this->L_id=id;
	pthread_mutex_init(&(this->Rmutex), NULL);
	pthread_mutex_init(&(this->Wmutex), NULL);
}
	//inicia el hilo del eqipo
void Equipo::start(int cantMensajes){
	this->m_recibidos=cantMensajes;
	this->m_enviados=cantMensajes;

	int rc 	= 	pthread_create(&this->thread, NULL,startEquipo, (void*) this );//se pasa como parametro el propio objeto
}
void Bridge::start(){
	int rc 	= 	pthread_create(&this->thread, NULL,startBridge, (void*) this  );//se pasa como parametro el propio objeto
}
//constuctores del puente
Bridge::Bridge(int id,LAN* l1,LAN* l2){

	this->B_id=id;
	this->lanUsadas=2;
	this->redes[0]=l1;
	this->redes[1]=l2;
}
Bridge::Bridge(int id,LAN* l1,LAN* l2,LAN* l3,int dirs[]){
	this->B_id=id;
	this->lanUsadas=3;
	this->redes[0]=l1;
	this->redes[1]=l2;
	this->redes[2]=l3;
	for (int i = 0; i < 16; ++i)
	{
		this->dir[i]=dirs[i];
	}
}
//definicion de funciones
void instrucciones() {
    printf("Para utilizar el programa, es necesario escribir: \n./nombrePrograma.o --on cantidadDeMensajes\n");
}

void iniciar(int cantidadDeMensajes){

    printf("Se inicia la ejecucion\n");
    archivo = fopen ("salida.txt","w");
    //se declaran las redes
    LAN red1(1);
    LAN red2(2);
    LAN red3(3);
    LAN red4(4);
    LAN red5(5);
    //direcciones a las cuales se debe enviar los mensajes por cada puente
    int dirsB1[16]={0,0,0,0,  1,1,1,1,   2,2,2,2,   2,2,2,2};
    int dirsB2[16]={2,2,2,2,   2,2,2,2   ,0,0,0,0,  1,1,1,1};
    //se declaran los puentes
    Bridge B1 (1,&red1,&red2,&red5,dirsB1);
    Bridge B2 (2,&red3,&red4,&red5,dirsB2);
    //se inician sis thread
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
    Equipo k(11,&red3);
    Equipo l(12,&red3);
    //red4
    Equipo m(13,&red4);
    Equipo n(14,&red4);
    Equipo o(15,&red4);
    Equipo p(16,&red4);
    //se inician los thread de cada equipo
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
    //se esperan los hilos para sincronizacion
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

    B1.wait();
    B2.wait();

    ordenDeTermino.pop_back();//elimina la ultima coma
    fprintf(archivo, "%s\n", ordenDeTermino.c_str());
	fclose (archivo);

}
//hilos
void* startEquipo(void* arg){
	Equipo* eq= (Equipo*) arg;//se obtiene el equipo
	//se obtiene la cola
	LAN* lan=eq->l;
	char numstr[21];
	int i=0;
	//se ejecuta mientras tenga mensajes para recibir y para enviar
	while(eq->m_enviados||eq->m_recibidos){
		//si le quedan mensajes para imprimir y puede tomar la linea
		if(eq->m_enviados&&!pthread_mutex_trylock(&(lan->Rmutex))){//se usa trylock para que no detenga la ejecucion y pueda leer en caso de no poder escribir
			//puede escribir solo si no hay otro mensaje en el buffer∫
			if(!pthread_mutex_trylock(&(lan->Wmutex))){
				//se reduce los mensajes a enviar
				eq->m_enviados--;
				int idR=eq->getID();
				if((i+1)%16==0)i++;//se salta un numero para que no se envie mensajes a si mismo
				int id_receptor=(idR+i)%16+1;//<<-- el modulo indica el numero maximo de nodos
				//<<-- formateo de el mensaje a enviar
				std::string s="Equipo ";
				int id=eq->getID();
				s+=(char)(64+id+((id>4)?(id>8)?2:1:0));
				s+=" manda mensaje a ";
				Mensaje m;
				m.id_receptor=id_receptor;
				m.buffer=s;
				char c=64+id_receptor+((id_receptor>4)?(id_receptor>8)?2:1:0);
				s+=c;
				s+=" mediante la LAN";

				sprintf(numstr, "%d", eq->l->L_id);
				s+= numstr;
				m.buffer=s;
				m.id_lan=eq->l->L_id;
				//se envia el mensaje
				lan->loadMessage(m);
				i++;//contador aumentado para enviar el siguiente
			}
			pthread_mutex_unlock(&(lan->Rmutex));//se suelta el semaforo de lectura pero no el de escritura
			continue;//se salta para darle oportunidad de cambio de contexto
		}
		//si puede recibir y puede tomar la linea entonces:
		if(eq->m_recibidos&&!pthread_mutex_trylock(&(lan->Rmutex))){
			//si el mensaje es para este equipo entonces:
			if( lan->hayMenssage( eq->getID() ) ){
				//obtengo el messaje
				std::string msg=lan->getMessage();
				//obtengo el id de la lan a utilizar
				int id_lan=lan->getIdLan();
				//se reduce la cantidad que debe recibir
				eq->m_recibidos--;
				int id=eq->getID();
				//se formatea la lan
				if(id_lan != eq->l->L_id){
					msg+=" y la LAN";
					sprintf(numstr, "%d", eq->l->L_id);
					msg+= numstr;
				}
				//se obtienen los puentes por los que a pasado el mensaje
				std::string puentes=lan->getPuentes();

				fprintf (archivo, "%s%s\n", msg.c_str(), puentes.c_str());
				//se desbloquea de escritura ya que el mensaje se a obtenido
				pthread_mutex_unlock(&(lan->Wmutex));
			}
			//se desbloquea la lectura
			pthread_mutex_unlock(&(lan->Rmutex));
		}
	}
	enEjecucion--;//quita uno de la ejecucion;
	//se bloquea para evitar problemas de concurrectia
	pthread_mutex_lock(&Omutex);
		int idR=eq->getID();
		char c=64+idR+((idR>4)?(idR>8)?2:1:0);
		ordenDeTermino+=c;//<<-- se agrega identificador a los equipos finalizados
		ordenDeTermino+=",";
	pthread_mutex_unlock(&Omutex);

	return NULL;
	
}

void* startBridge(void* arg){
	Bridge* b=(Bridge*) arg;//se castea el propio puente
	while(enEjecucion){//mientras existan equipos en ejecucion
		//escritura
		if(b->hayMensajes()){//si hay mensajes en la cola
			Mensaje m=b->tomarMensaje();//se toma el mensaje de la cola
			int lanDestino=b->dir[m.id_receptor-1];
			LAN* lan=b->redes[lanDestino];
			if(!pthread_mutex_trylock(&(lan->Rmutex))){//si puede tomar la linea para lectura
				if(!pthread_mutex_trylock(&(lan->Wmutex))){//si puede tomar la linea para escritura
					//se agrega que paso por el puente
					m.bufferPuente+=(b->B_id==1)?", pasa por el puente 1":", pasa por el puente 2";
					//se carga el mensaje
					lan->loadMessage(m);
				}else{
					b->encolarMensaje(m);//devolverlo porque no se pudo enviar
				}
				pthread_mutex_unlock(&(lan->Rmutex));
			}else{
				b->encolarMensaje(m);//devolverlo porque no se pudo leer la linea
			}
		}	
		//lectura lee itrativamente las tres lan conectadas
		for (int i = 0; i < 3; ++i)
		{
			int id_R=b->redes[i]->getIdReceptor();
			if(id_R){
				//si puede tomar la linea
				if(!pthread_mutex_trylock(&(b->redes[i]->Rmutex))){
					//
					LAN* lan=b->redes[i];
					if(b->dir[id_R-1]!=i)//deberia comprobarse para todos los que no pertenecen a la red 0
					{	
						//se obtiene el mensaje
						std::string s=lan->getMessage();
						Mensaje m;// se crea una nuesva estructira mensaje
						m.buffer=s;
						m.id_receptor=id_R;
						m.bufferPuente=lan->getPuentes();
						b->encolarMensaje(m);//se agrega el mensaje a la cola para ser enviado
						//se desbloquea la linea para evitar deadlock
						pthread_mutex_unlock(&(b->redes[i]->Wmutex));
					}
					//se suelta la linea para lectura
					pthread_mutex_unlock(&(b->redes[i]->Rmutex));
				}
			}
		}
		
	}
	//el puente finaliza una vez todos los equipos terminan de ejecutarse
	return NULL;
}