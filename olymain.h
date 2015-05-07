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
#include "console.h"
#include "messenger.h"
#include "preferencias.h"
#include "protocolo.h"
#include "verifytime.h"
#include "swarm.h"

#define option_connect        0
#define option_disconnect     1
#define option_start          2
#define option_stop           3
#define option_manual_control 4
#define option_exit           5

#define n_options 6
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
private slots:

    void connection();
    void disconnection();
    void begin();
    void stop();
    void openAbout();
    void recieveInformation();
signals:
    void stopTimer();
private:
    Ui::olymain *ui;
    int num_robots;
    bool manual_control;
    Console *board;
    Console *terminal;
    QSerialPort *serial;
    messenger *sender;
    QToolButton **options;
    preferencias *settings;
    QQueue<QString> *messages_queue;
    QThread thread;
    QTimer timer;
    verifyTime *checker;
    swarm *swarm_object;
    //Variables para verificar que es seguro usar el objeto que envia y la cola de mensajes
    bool sender_safe;
    bool queue_safe;
    QString var;

};

#endif // OLYMAIN_H
