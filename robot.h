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
    ~robot();
private:
    QVBoxLayout *things_layout;
    Console *board; //Las consolas se usarán como pizarrones
    QGroupBox *exceptions_group;
    QCheckBox *sensor_infrarojo;
    QCheckBox *sensor_ultrasonido;
    QLabel *actual_behavior;
    QString name;
    char identificator;
    char behave;
    bool *exceptions;
};

#endif // ROBOT_H
