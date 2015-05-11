#ifndef ROBOT_H
#define ROBOT_H
#include <QGroupBox>
#include <QtCore/QDebug>
#include <console.h>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <iostream>
#include <string>
#include <QQueue>
#include "messenger.h"
#include "protocolo.h"
#define _main 0
#define secondary 1
#define n_behaves 2
#define none '-'

using namespace std;
class robot : public QGroupBox{
      Q_OBJECT
public:
    robot(QString name, char identificator, char default_behave, QQueue<QString>* messages_queue, bool *queue_safe, QWidget *parent = 0);
    QString getName();
    char getIdentificator();
    //0 ultra, 1 infra
    bool getException(int exception_tipe);
    /*Se ejecuta la instrucción que llega*/
    void processOrder(QString data);
    /*Se cambia el comportamiento actual, NOTA: si es alguno de los comportamientos definidos en seguir instrucciones el comportamiento cambia automaticamente*/
    void setBehave(string new_behave);
    /*Es necesario estar en modo de seguir instrucciones*/
    /*Si la pudo cambiar retorna true, si no esta en el modo correcto o hay algo mal con exception_type retornará false*/
    bool setException(int exception_type, bool option);
    /*Retorna el comportamiento actual*/
    char getBehave();
    /*TODO: Para buscar el robot levantar un emit aqui cuando responda*/
    void find();
    ~robot();
private:
    QVBoxLayout      *things_layout;
    Console          *board; //Las consolas se usarán como pizarrones
    QGroupBox        *exceptions_group;
    QCheckBox        **sensores;
    QLabel           *actual_behavior;
    QString          name;
    QQueue<QString>* messages_queue;
    char             identificator;
    char             *behave;
    bool             *exceptions;
    bool             *queue_safe;
    bool             *sender_safe;
};

#endif // ROBOT_H
