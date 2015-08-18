#include "control_manual.h"

control_manual::control_manual(QString robot_name, QWidget *parent) : QMainWindow(parent){
    this->setWindowTitle("Control manual de " + robot_name);
    this->setFixedSize(455, 220);
    this->setStyleSheet("background: rgb(189, 195, 199);");
    QWidget *window                         = new QWidget();
    QGroupBox *sensors_container            = new QGroupBox("", this);
    QGroupBox *behave_container             = new QGroupBox("", this);
    QGroupBox *arrows_container             = new QGroupBox("", this);
    QHBoxLayout *first_layout               = new QHBoxLayout();
    QVBoxLayout *behave_sensors_layout      = new QVBoxLayout();
    QHBoxLayout *sensors_layout             = new QHBoxLayout();
    QHBoxLayout *behave_layout              = new QHBoxLayout();
    behaviors                               = new QComboBox(this);


    sensors_buttons = new QToolButton*[n_buttons_sensors];
    for(int index = 0; index < n_buttons_sensors; index++) {
        sensors_buttons[index] = new QToolButton(this);
        sensors_buttons[index]->setIconSize(QSize(50, 50));
        sensors_buttons[index]->setToolButtonStyle(Qt::ToolButtonIconOnly);
        sensors_buttons[index]->setStyleSheet("QToolButton {\
                                                background-color: rgb(67, 97, 102);\
                                                border-radius: 50%;\
                                                width:  70px;\
                                                height: 70px;\
                                                position: centered;} \
                                                QToolButton:pressed { \
                                                background-color: rgb(67, 97, 102);}");
        sensors_layout->addWidget(sensors_buttons[index]);
    }
    behave_layout->addWidget(behaviors);
    //arrows_container->setFixedSize(100, 100);
    arrows_container->setStyleSheet("QGroupBox {\
                                     background: rgb(255, 255, 255); \
                                     border-radius: 90%;\
                                     width:  200px;\
                                     height: 200px;\
                                     position: centered;}");

    sensors_container->setStyleSheet("QGroupBox {\
                                     background: rgb(51, 110, 123); width:  100px;\
                                     height: 200px;}");
    behave_container->setStyleSheet("QGroupBox {\
                                     background: rgb(92, 151, 191); }");


    sensors_container->setLayout(sensors_layout);
    behave_container->setLayout(behave_layout);
    behave_sensors_layout->addWidget(sensors_container);
    behave_sensors_layout->addWidget(behave_container);
    first_layout->addLayout(behave_sensors_layout);
    first_layout->addWidget(arrows_container);
    window->setLayout(first_layout);
    setCentralWidget(window);
}

control_manual::~control_manual()
{

}

