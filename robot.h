#ifndef ROBOT_H
#define ROBOT_H
#include <QGroupBox>
#include <QtCore/QDebug>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QToolButton>
#include <iostream>
#include <string>
#include <QQueue>

#include "control_manual.h"
#include "console.h"
#include "messenger.h"
#include "protocolo.h"
#define _main 0
#define secondary 1
#define n_behaves 2
#define none '-'

using namespace std;
class robot : public QGroupBox{
    //friend class  control_manual;
      Q_OBJECT
public:
    robot(QString name, char identificator, char default_behave, QQueue<QString>* messages_queue, bool *queue_safe, QWidget *parent = 0);
    QString getName();
    char getIdentificator();
    //0 ultra, 1 infra
    bool getException(int exception_tipe);
    /*Se ejecuta la instrucción que llega*/
    void operator<<(QString data);


    /*Se cambia el comportamiento actual, NOTA: si es alguno de los comportamientos definidos en seguir instrucciones el comportamiento cambia automaticamente*/
    bool setBehave(char behave, char opcion);

    void setConnections();
    /*TODO: Para buscar el robot levantar un emit aqui cuando responda*/
    void find();



    bool cambiarVelocidad(int nueva_velocidad);

    /*Comportamientos complejos*/
    /*Crear como un hilo que verifique como cambia el estado del robot
     *  y que se lo envie al comportamiento complejo definido aquí*/

    control_manual* getControl();

    ~robot();
private slots:
    /*Si la pudo cambiar retorna true, si no esta en el modo correcto o hay algo mal con exception_type retornará false*/
    bool setException(int exception_type, bool option);
    bool setBehave(char behave);
    /*Comportamientos básicos*/
    bool evadir();
    bool detener();
    bool seguirInstrucciones(char opcion);
    bool seguirInstrucciones();
    bool explorar();

private:
    QVBoxLayout      *things_layout;
    QHBoxLayout      *half_layout;
    QHBoxLayout      *sensors_layout;
    QVBoxLayout      *control_layout;
    QLabel          **sensors;
    QLabel           *robot_nombre;
    QToolButton      *control_manual_boton;
    Console          *board; //Las consolas se usarán como pizarrones
    QLabel           *actual_behavior;
    QString           name;
    QQueue<QString>*  messages_queue;
    char              identificator;
    QString           old_m;
    char             *behave;
    bool             *exceptions;
    bool             *queue_safe;
    bool             *sender_safe;
    control_manual   *control;
};

#endif // ROBOT_H
