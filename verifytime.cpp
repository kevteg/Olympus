#include "verifytime.h"

verifyTime::verifyTime(Console *terminal, swarm *swarm_object, bool *sender_safe, bool *queue_safe, messenger *sender, QQueue<QString> *messages_queue, QObject *parent) : QObject(parent){
    this->sender_safe = sender_safe;
    this->queue_safe = queue_safe;
    this->sender = sender;
    this->messages_queue = messages_queue;
    this->swarm_object = swarm_object;
    this->terminal = terminal;
}

verifyTime::~verifyTime(){

}

void verifyTime::onTimeout(){
   if(sender_safe && queue_safe){
       *sender_safe = false;
       *queue_safe = false;
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
       *sender_safe = true;
       *queue_safe = true;
   }
}
