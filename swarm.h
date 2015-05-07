#ifndef SWARM_H
#define SWARM_H
#include <iostream>
#include <vector>
#include "robot.h"
/*Este objeto se usa como la colectividad de individuos. Donde los robots son los agentes*/
class swarm
{
public:
    swarm();
    ~swarm();
    vector <robot*>* getRobots();
    /*Si al robot al que se le intenta enviaro del que se esta recibiendo existe y el mensaje esta bien se retorna el nombre del robot*/
    QString sendData(QByteArray data);
private:
    vector <robot*> *robots;
};

#endif
