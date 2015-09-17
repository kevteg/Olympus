#include "swarm.h"

swarm::swarm(QObject *parent) : QObject(parent){
    robots = new vector <robot*>;
}

swarm::~swarm(){
    for(vector<robot*>::iterator r =  robots->begin(); r != robots->end(); ++r)
        delete *r;
    delete robots;
}
vector<robot*> *swarm::getRobots(){
    return robots;
}
void swarm::closeControls(){
    for(vector<robot*>::iterator r =  robots->begin(); r != robots->end(); ++r)
        (*r)->getControl()->close();
}
void swarm::rutine(){
    /*Sí está habilitado enviar es porque está activo las rutinas de los robots*/
    if(protocolo::send_enabled){

        if(robots->at(1)->getException(protocolo::sensor_distancia) && !robots->at(0)->getException(protocolo::sensor_distancia)){
            robots->at(1)->detener();
            robots->at(0)->seguirInstrucciones(protocolo::derecha);
        }

        if(robots->at(0)->getException(protocolo::sensor_distancia) && !robots->at(1)->getException(protocolo::sensor_distancia)){
            robots->at(0)->detener();
            robots->at(1)->seguirInstrucciones(protocolo::izquierda);
        }
    }else
        qDebug() << "Pausa de rutinas";
}

QString swarm::sendData(QByteArray data){
    QString name = "err";
    /*Si el mensaje no cumple el protocolo o si no existe el robot se envia err*/
    if(protocolo::verificacion(data, 0))
        for(vector<robot*>::iterator r = robots->begin(); r != robots->end(); ++r)
           if((*r)->getIdentificator() == data.at(1)){
               *(*r) << QString::fromLatin1(data);
               name = (*r)->getName();
           }
    return name;
}
