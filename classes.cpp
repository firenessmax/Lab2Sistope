
#include "classes.h"

//metodos de equipo
//constructor
Equipo::Equipo(int id,LAN* l,IPuente* p){
   thread_id=id;
   this->l=l;
   this->p=p;
   nombre="jarvis";
}

void Equipo::start(){
   int result_code = pthread_create(&this->this_thread, NULL, Equipo::onStart, (void *) &this->thread_id);
   //std::cout<<result_code<<std::endl;
}
void Equipo::sendMessage(std::string msg, Equipo* receptor){
   //seccion critica
   if(adyacencia[this->thread_id-1][receptor->thread_id-1]){
      l->loadMessage(msg);
      receptor->reciveMessage();
   }else{
      this->sendMessagethroughBridge(msg,this->p,receptor);
   }
      //fin seccion critica... creo;
}
void Equipo::sendMessagethroughBridge(std::string msg,IPuente* p, Equipo* receptor){
   p->repiteMessage(msg,receptor);
}
void Equipo::reciveMessage(){

   printf("soy %d y recibí %s\n",this->thread_id,&(l->getMessage())[0u]);
}
void IPuente::repiteMessage(IEquipo* emisor,IEquipo* receptor){
   	//std::cout<<"repitiendo..."<<std::endl;
	std::string mensaje=msg;
	
}