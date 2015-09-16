#ifndef OLYMAIN_H
#define OLYMAIN_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <qmessagebox.h>
#include <QToolButton>
#include <QtCore>
#include <sstream>
#include <QSerialPort>
#include "verifytime.h"
#include "console.h"
#include "messenger.h"
#include "preferencias.h"

#define option_connect_disconnect 0
#define option_start_stop         1
#define option_preferences        2
#define option_about              3
#define option_exit               4

#define n_options 5
using namespace std;

namespace Ui {
class olymain;
}

class olymain : public QMainWindow
{
    Q_OBJECT

public:
    explicit olymain(QWidget *parent = 0);
    void addRobots();
    bool openPreFile();
    ~olymain();
    void setConections();
    void defaultSituation();
    void connection();
    void disconnection();
    void begin();
    void stop();
private slots:
    void connect_serial();
    void robotRutine();
    void openAbout();
    void recieveInformation();
    void changeMessState();
    void change_info(preferencias::preferencia set);
signals:
    void stopTimer();
protected:
    virtual void closeEvent(QCloseEvent *);
private:
    Ui::olymain               *ui;
    int                        num_robots;
    int                        count_onoff; //Saber cuantas veces se ha encencido o apagado las rutinas
    bool                       manual_control;
    Console                   *board;
    Console                   *terminal;
    QSerialPort               *serial;
    messenger                 *sender;
    QToolButton              **options;
    preferencias              *settings;
    preferencias::preferencia  opt;
    QQueue<QString>           *messages_queue;
    QThread                    sender_thread;
    QThread                    rutine_thread;
    QTimer                     sender_timer;
    QTimer                     rutine_timer;
    verifyTime                *checker;
    swarm                     *swarm_object;
    QJoystick                 *joystick;
    QLabel                    *cant_robots;
    //Variables para verificar que es seguro usar el objeto que envia y la cola de mensajes
    bool                       serial_connection;
    bool                       rutine_robots;
    QString                    var;
    bool                       show_comming_info;

};

#endif // OLYMAIN_H
