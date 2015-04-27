#ifndef OLYMAIN_H
#define OLYMAIN_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include "robot.h"
#include <QDebug>
#include <fstream>
#include <string>
#include <qmessagebox.h>
#include <QToolButton>
#include "console.h"
#include <sstream>
#include "messenger.h"
#include <QSerialPort>
#include "preferencias.h"
#include "protocolo.h"

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

private:
    Ui::olymain *ui;
    int num_robots;
    bool manual_control;
    vector <robot*> robots;
    Console *board;
    Console *terminal;
    QSerialPort *serial;
    messenger *sender;
    QToolButton **options;
    preferencias *settings;
};

#endif // OLYMAIN_H
