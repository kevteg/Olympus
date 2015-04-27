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
#include "messenger.h"
#include "protocolo.h"

using namespace std;
class robot : public QGroupBox{
      Q_OBJECT
public:
    robot(QString name, char identificator, char default_behave, messenger *sender, QWidget *parent = 0);
    string getName();
    char getIdentificator();
    //0 ultra, 1 infra
    bool getException(int exception_tipe);
    /*Se ejecuta la instrucción que llega*/
    void processOrder(string data);

    ~robot();
private:
    QVBoxLayout *things_layout;
    Console *board; //Las consolas se usarán como pizarrones
    QGroupBox *exceptions_group;
    QCheckBox *sensor_infrarojo;
    QCheckBox *sensor_ultrasonido;
    QLabel *actual_behavior;
    string name;
    char identificator;
    char behave;
    bool *exceptions;
};

#endif // ROBOT_H
