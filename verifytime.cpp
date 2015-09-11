#include "verifytime.h"

verifyTime::verifyTime(Console *terminal, swarm *swarm_object, messenger *sender, QQueue<QString> *messages_queue, QObject *parent) : QObject(parent){
    this->sender = sender;
    this->messages_queue = messages_queue;
    this->swarm_object = swarm_object;
    this->terminal = terminal;
}

verifyTime::~verifyTime(){

}

void verifyTime::onTimeout(){
   if(protocolo::sender_safe && protocolo::queue_safe && protocolo::send_enabled){
       protocolo::sender_safe = false;
       protocolo::queue_safe  = false;
       QString robot_name;
       if(!messages_queue->isEmpty() && sender->getSerial()->isOpen()){
           QString mensaje = messages_queue->dequeue();
           qDebug()<<"Hilo: "<<QThread::currentThreadId() << " intenta enviar: " << mensaje;
           robot_name = swarm_object->sendData(mensaje.toLatin1());
           /*Antes de enviar se verifica que lo que este en la cola. Si esta bien el mensaje se envia*/
           if(robot_name != "err"){
                sender->sendMessage(mensaje.toLatin1());
                terminal->putData(QString(QString::fromLatin1("Se envía: ") + mensaje + " a " + (robot_name.toLatin1()) + "\n").toLatin1());
                qDebug() << "Envio correcto";
           }else{
                terminal->putData(QString("Error[6]: mensaje no cumple protocolo: "+mensaje+"\n").toLatin1());
                qDebug() << "Envio fallido";
           }
       }
       protocolo::sender_safe = true;
       protocolo::queue_safe  = true;
   }
}

