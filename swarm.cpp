#include "swarm.h"

swarm::swarm(){
    robots = new vector <robot*>;
}

swarm::~swarm(){
    delete robots;
}
vector<robot*> *swarm::getRobots(){
    return robots;
}

QString swarm::sendData(QByteArray data){
    QString name = "err";
    /*Si el mensaje no cumple el protocolo o si no existe el robot se envia err*/
    if(verifyMessage(data, 0))
        for(vector<robot*>::iterator r = robots->begin(); r != robots->end(); ++r)
           if((*r)->getIdentificator() == data[1]){
               (*r)->processOrder(QString(data));
               name = (*r)->getName();
           }
    return name;
}
bool swarm::verifyMessage(QByteArray data, int index){
    /*5 es el tamaño mínimo de los mensajes*/
    if(index < data.size() && data.size() >= protocolo::tam_min){
        //qDebug() << data[index];
        switch(data.at(index)){
            case protocolo::delimitador_i:
                if(!(index == 0
                   &&
                   ((index + 1) < data.size() &&
                    data.at(index + 1) != protocolo::delimitador_i &&
                    data.at(index + 1) != protocolo::delimitador_f &&
                    data.at(index + 1) != protocolo::separador)))
                    return false;
            break;
            case protocolo::delimitador_f:
                if(index == (data.size() - 1) &&
                   (index - 1 > 0 &&
                    data.at(index - 1) != protocolo::delimitador_i &&
                    data.at(index - 1) != protocolo::delimitador_f &&
                    data.at(index - 1) != protocolo::separador))
                    return true;
            break;
            case protocolo::separador:
                if(((index + 1) < data.size() &&
                     data.at(index + 1) == protocolo::delimitador_i ||
                     data.at(index + 1) == protocolo::delimitador_f ||
                     data.at(index + 1) == protocolo::separador)
                   ||
                    ((index - 1) > 0 &&
                     data.at(index - 1) == protocolo::delimitador_i ||
                     data.at(index - 1) == protocolo::delimitador_f ||
                     data.at(index - 1) == protocolo::separador))
                    return false;
            break;
            default:
                if((index == 0 || index == (data.size() - 1)) ||
                  ((index - 1 > 0 &&
                   data.at(index - 1) != protocolo::delimitador_i &&
                   data.at(index - 1) != protocolo::separador)
                   ||
                   (index + 1 < data.size() &&
                   data.at(index + 1) != protocolo::delimitador_f &&
                   data.at(index + 1) != protocolo::separador)))
                   return false;
            break;
        }
        return verifyMessage(data, index + 1);
    }
return false;
}

