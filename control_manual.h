#ifndef CONTROL_MANUAL_H
#define CONTROL_MANUAL_H

#include <iostream>
#include <QMainWindow>
#include <QToolButton>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtCore/QDebug>
#include <QChar>
#include <QKeyEvent>
#include <string>
#include <fstream>
#include <sstream>
#include <QSpacerItem>
#include "protocolo.h"
#include "qjoystickenumerator.h"

#define n_buttons_options 3
#define n_arrows          4
#define OPC_ULTRA         0
#define OPC_SEARCH        1
#define OPC_INFRA         2

using namespace std;
class control_manual : public QMainWindow{
   // friend class  robot;
    Q_OBJECT
public:
    explicit control_manual(QString robot_name, QWidget *parent = 0);
    void connect_joy(QJoystick *joystick);
    ~control_manual();
    void setConnections();
signals:
    void movement(char);
    void behave(char);
    void exc(int, bool);
    void find();
private slots:
    void updateAxis(int, short);
    void updateButton(int, bool);
    void buttonInfra();
    void buttonUltra();
    void buttonSearch();
    void buttonUp();
    void buttonDown();
    void buttonRight();
    void buttonLeft();
    void changeBehave();
protected:
     virtual void keyPressEvent(QKeyEvent *e);
private:
    QToolButton     **arrows_buttons;
    QToolButton     **options_buttons;
    QComboBox        *behaviors;
    QString          robot_name;
    QJoystick        *joystick;
    bool             *active_options;

};

#endif // CONTROL_MANUAL_H
