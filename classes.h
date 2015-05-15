#ifndef LAB2SISTOPE
#define LAB2SISTOPE
class LAN
{
private:
   std::string buffer;
public:
   void loadMessage(std::string msg){this->buffer=msg;}
   std::string getMessage(){return this->buffer;}
};



class IEquipo//interface para el equipo
{public:
   IEquipo();
   void reciveMessage();
};
IEquipo::IEquipo(){}
class IPuente//interface para el puente
{
public:
   void repiteMessage(std::string msg,IEquipo* receptor);
};
class Equipo:public IEquipo{
   protected:
   LAN* l;
   std::string nombre;
   IPuente* p;
   int thread_id;
   pthread_t this_thread;
   public:
      Equipo(int id,LAN* l,IPuente* p);
      static void* onStart(void* arg){
         //std::cout<<"holi soy : "<<*(int*) arg<<std::endl;
         //printf("holi soy : %d \n",*(int*) arg);
         return NULL;
      }
      void start();
      void wait(){pthread_join(this->this_thread, NULL);}
      void sendMessage(std::string msg, Equipo* receptor);
      void sendMessagethroughBridge(std::string msg,IPuente* p, Equipo* receptor);
      void reciveMessage();
      int getID(){return this->thread_id;}
};
class Puente:public IEquipo, public IPuente{
public:
   int id_puente;
   LAN** redes;
   Puente(int id){this->id_puente=id;}
   Puente(int id,LAN[] l){this->id_puente=id;this->redes=l;}
   void repiteMessage(std::string msg, IEquipo* receptor);
};

#endif