//librerias C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
//librerias cpp
#include <iostream>
#include <vector>
#include <string>
//definicion de clases
typedef struct m
{
    std::string buffer;
    int id_receptor;
} Mensaje;


class LAN//clase para el paso de mensajes
{
private:
    Mensaje M_buffer;
    std::string buffer;
public:
    pthread_mutex_t Lmutex;//equivalente a declararlo e inicializarlo
    pthread_mutex_t Rmutex;
    pthread_mutex_t Wmutex;
    int id_receptor;
    LAN();
    void loadMessage(Mensaje M_buffer){
        this->M_buffer=M_buffer;
    }
    void loadMessage(int id_receptor, std::string msg){
        this->id_receptor=id_receptor;this->buffer=msg;
    }
    int hayMenssage(int eq_id){return eq_id==this->M_buffer.id_receptor;}
    std::string getMessage(){
        M_buffer.id_receptor=0;
        return this->M_buffer.buffer;
    }
    int getIdReceptor(){
        printf("tracking :| 42 |getIdReceptor()\n");
        int r=M_buffer.id_receptor;
        printf("out tracking :| 43 |getIdReceptor()\n");
        return r;
    }

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
        void reciveMessage();//irr
        int getID(){return this->E_id;}
};

class Bridge{
        pthread_t thread;
        std::vector<Mensaje> cola;
    public:
        int B_id;
        int m_recibidos;
        int m_enviados;
        int lanUsadas;
        int dir[3];
        LAN* redes[3];
        Bridge(int id,LAN* l1,LAN* l2);
        Bridge(int id,LAN* l1,LAN* l2,LAN* l3,int dirs[]);
        void encolarMensaje(Mensaje m){this->cola.push_back(m);}
        Mensaje tomarMensaje(){
            Mensaje m=this->cola.front();
            this->cola.erase(this->cola.begin());
            return m;
        }
        void devolverMensaje(Mensaje m){this->cola.insert(this->cola.begin(),m);}
        int hayMensajes(){
            //if(this->cola.size()>0)printf("%d\n",(int)this->cola.size());
            return this->cola.size()>0;}
        void start();
        void wait(){pthread_join(this->thread, NULL);}
};

//prototipo de funciones
//la idea es hacer que las funciones que daban referencias cruzadas hacerlas al estilo C 


void* startEquipo(void* arg);
void* startBridge(void* arg);
void sendMessage( Equipo* emisor,Equipo* receptor, std::string msg);

int cantidadDeDatosPorBridge(int n);
void iniciar(int cantidadMensajes);
void instrucciones();

//
int indices[3]={1,1,2};

int main(int argc, char *argv[]){ 
    //int laCantidadDeDatosPorBridge=cantidadDeDatosPorBridge(cantidadDeMensajes);
    //printf("laCantidadDeDatosPorBridge: %d\n", laCantidadDeDatosPorBridge);
    int cantidadDeMensajes=1;

    LAN red1,red2,red3,red4,red5;
    int dirsB1[16]={0,0,0,0,  1,1,1,1,   2,2,2,2,   2,2,2,2};
    int dirsB2[16]={2,2,2,2,   2,2,2,2   ,0,0,0,0,  1,1,1,1};
    Bridge B1 (1,&red1,&red2,&red5,dirsB1);
    Bridge B2 (2,&red3,&red4,&red5,dirsB2);
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
    return 0;
}

//clases
LAN::LAN(){
    this->M_buffer.id_receptor=0;
    this->id_receptor=0;
    pthread_mutex_init(&(this->Lmutex), NULL);
    pthread_mutex_init(&(this->Rmutex), NULL);
    pthread_mutex_init(&(this->Wmutex), NULL);
}
void Equipo::start(int cantMensajes){
    this->m_recibidos=cantMensajes;
    this->m_enviados=cantMensajes;
    int rc  =   pthread_create(&this->thread, NULL,startEquipo, (void*) this );
    //printf("Hello World! It's me, thread with argument %d!\n", this->E_id);
}
void Bridge::start(){
    
    int rc  =   pthread_create(&this->thread, NULL,startBridge, (void*) this  );
    //printf("Hello World! It's me, thread with argument %d!\n", this->B_id);
}
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
//funciones
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

    
}
//hilos
void* startEquipo(void* arg){
    Equipo* eq= (Equipo*) arg;
    LAN* lan=eq->l;
    int i=0;
    while(eq->m_enviados||eq->m_recibidos){
        if(eq->m_enviados&&!pthread_mutex_trylock(&(lan->Rmutex))){
            if(!pthread_mutex_trylock(&(lan->Wmutex))){
                eq->m_enviados--;
                int id_receptor=(eq->getID()+i)%3+1+i/16;//<<-- el modulo indica el numero maximo de nodos
                std::string s="";
                s+=(char)(64+eq->getID());
                s+="->";
                Mensaje m;
                m.id_receptor=id_receptor;
                m.buffer=s;
                lan->loadMessage(m);
                printf("(%s%c)\n", s.c_str(),(char)(64+id_receptor));
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
    //entrada1
    while(true){//revisar la condicion de termino... yo creo que parecido a los anteriores quizas contando los que corresponde y los del buffer algo asi
        //escritura
        if(b->hayMensajes()){//si hay mensajes en la cola
            Mensaje m=b->tomarMensaje();
            int lanDestino=b->dir[m.id_receptor-1];
            LAN* lan=b->redes[lanDestino];
            if(!pthread_mutex_trylock(&(lan->Rmutex))){
                //printf("disponiendose a repetir\n");
                if(!pthread_mutex_trylock(&(lan->Wmutex))){
                    //eq->m_enviados--;
                    //printf("B1(%s%c)\n", m.buffer.c_str(),(char)(64+m.id_receptor));
                    lan->loadMessage(m);
                    
                    //i++;
                }else{
                    //printf("oow\n");
                    b->encolarMensaje(m);//devolverlo
                }
                pthread_mutex_unlock(&(lan->Rmutex));
            }else{
                b->encolarMensaje(m);//devolverlo
            }
        }   
        //lectura
        for (int i = 0; i < 3; ++i)
        {
            int id_R=b->redes[i]->getIdReceptor();
            printf("trakinkg 2\n");
            if(id_R){

                //printf("(?)->%c\n",(char)(id_R+64));
                if(!pthread_mutex_trylock(&(b->redes[i]->Rmutex))){
                    //printf("dir[C]=%d\n",b->dir[id_R]);
                    LAN* lan=b->redes[i];
                    if(b->dir[id_R-1]!=i)//deberia comprobarse para todos los que no pertenecen a la red 0
                    {   
                        std::string s=lan->getMessage();
                        Mensaje m;
                        m.buffer=s;
                        m.id_receptor=id_R;
                        b->encolarMensaje(m);
                        //printf("encolando: %s%c\n",&s[0u],(char)(64+id_R));
                        pthread_mutex_unlock(&(b->redes[i]->Wmutex));
                    }
                    pthread_mutex_unlock(&(b->redes[i]->Rmutex));
                }
            }
        }
        
    }
    return NULL;
}