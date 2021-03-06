#ifndef SWARM_H
#define SWARM_H

#include <QObject>
#include <iostream>
#include <vector>
#include "robot.h"
/*Este objeto se usa como la colectividad de individuos. Donde los robots son los agentes*/
class swarm : public QObject{
    Q_OBJECT
public:
    swarm( QObject *parent = 0);
    ~swarm();
    vector <robot*>* getRobots();
    /*Si al robot al que se le intenta enviaro del que se esta recibiendo existe y el mensaje esta bien se retorna el nombre del robot*/
    QString sendData(QByteArray data);
    void closeControls();
public slots:
    void rutine();
private:
    vector <robot*> *robots;
};

#endif

