#include "control_manual.h"

control_manual::control_manual(QString robot_name, QWidget *parent) : QMainWindow(parent){
    int index;
    this->robot_name = robot_name;
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
    QHBoxLayout *arrows_layout              = new QHBoxLayout();
    QVBoxLayout *arrows_layout_rl           = new QVBoxLayout();
    behaviors                               = new QComboBox(this);
    joystick                                = NULL;
    active_options                            = new bool[n_buttons_options];
    options_buttons                         = new QToolButton*[n_buttons_options];
    arrows_buttons                          = new QToolButton*[n_arrows];
    behaviors->setWhatsThis("Cambiar comportamiento actual");

    for (int index = 0; index < n_buttons_options; index++)
        active_options[index] = false;


    for(index = 0; index < n_buttons_options; index++) {
        QChar img;
        options_buttons[index] = new QToolButton(this);
        options_buttons[index]->setIconSize(QSize(100, 100));
        img = !index?'U':((index == 1)?'B':'I');
        options_buttons[index]->setIcon(QIcon(":/images/Imagenes/" + QString(img) + "1.png"));
        options_buttons[index]->setToolButtonStyle(Qt::ToolButtonIconOnly);
        options_buttons[index]->setStyleSheet("QToolButton {\
                                                background-color: rgb(51, 110, 123);\
                                                border-radius: 50%;\
                                                width:  70px;\
                                                height: 70px;\
                                                position: centered;} \
                                                QToolButton:pressed { \
                                                background-color: rgb(67, 97, 102);}");
        options_buttons[index]->setEnabled((!index || index == n_buttons_options - 1)?false:true);
        sensors_layout->addWidget(options_buttons[index]);
    }


    for(index = 0; index < n_arrows; index++) {
        stringstream numero;
        numero << index + 1;
        arrows_buttons[index] = new QToolButton(this);
        arrows_buttons[index]->setIconSize(QSize(100, 100));
        string dir = string(":/images/Imagenes/arrow") + numero.str() + string(".png");
        arrows_buttons[index]->setIcon(QIcon(QString::fromStdString(dir)));
        arrows_buttons[index]->setToolButtonStyle(Qt::ToolButtonIconOnly);
        arrows_buttons[index]->setStyleSheet("QToolButton {\
                                                background-color: rgb(255, 255, 255);\
                                                border-radius: 50%;\
                                                width:  70px;\
                                                height: 70px;\
                                                position: centered;} \
                                                QToolButton:pressed { \
                                                background-color: rgb(238, 238, 238);}");
        if(!index || index == 3)
            arrows_layout->addWidget(arrows_buttons[index]);
        else{
            if(index == 1)
                arrows_layout->addLayout(arrows_layout_rl);
            arrows_layout_rl->addWidget(arrows_buttons[index]);
        }
        arrows_buttons[index]->setEnabled(false);
    }
    behave_layout->addWidget(behaviors);

    arrows_container->setStyleSheet("QGroupBox {\
                                     background: rgb(255, 255, 255); \
                                     border-radius: 90%;\
                                     width:  200px;\
                                     height: 200px;\
                                     position: centered;}");
    arrows_container->setAlignment(Qt::AlignCenter);
    sensors_container->setStyleSheet("QGroupBox {\
                                     background: rgb(51, 110, 123); width:  100px;\
                                     height: 200px;}");
    behave_container->setStyleSheet("QGroupBox {\
                                     background: rgb(92, 151, 191); }");
    index = 0;
    for (char com = 'A'; index < protocolo::total_comportamientos; com++, index++)
        behaviors->addItem(protocolo::getCadenaInstruccion(com), (com == 'D')?'!':com);

    arrows_container->setLayout(arrows_layout);
    sensors_container->setLayout(sensors_layout);
    behave_container->setLayout(behave_layout);
    behave_sensors_layout->addWidget(sensors_container);
    behave_sensors_layout->addWidget(behave_container);
    first_layout->addLayout(behave_sensors_layout);
    first_layout->addWidget(arrows_container);
    window->setLayout(first_layout);
    behaviors->setFocusPolicy(Qt::NoFocus);
    setCentralWidget(window);
    setConnections();
}
void control_manual::connect_joy(QJoystick *joystick){
    qDebug() << "SI";
    this->joystick = joystick;
    connect(this->joystick, SIGNAL(axisChanged(int,short)), this, SLOT(updateAxis(int,short)));
    connect(this->joystick, SIGNAL(buttonChanged(int,bool)), this, SLOT(updateButton(int,bool)));
}
void control_manual::setConnections(){
    connect(options_buttons[OPC_ULTRA], SIGNAL(clicked()), this, SLOT(buttonUltra()));
    connect(options_buttons[OPC_SEARCH], SIGNAL(clicked()), this, SLOT(buttonSearch()));
    connect(options_buttons[OPC_INFRA], SIGNAL(clicked()), this, SLOT(buttonInfra()));
    connect(arrows_buttons[protocolo::up], SIGNAL(clicked()), this, SLOT(buttonUp()));
    connect(arrows_buttons[protocolo::down], SIGNAL(clicked()), this, SLOT(buttonDown()));
    connect(arrows_buttons[protocolo::right], SIGNAL(clicked()), this, SLOT(buttonRight()));
    connect(arrows_buttons[protocolo::left], SIGNAL(clicked()), this, SLOT(buttonLeft()));
    connect(behaviors, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBehave()));
}

void control_manual::buttonUp(){
    qDebug() << "Adelante";
    emit movement(protocolo::adelante);
}

void control_manual::buttonDown(){
    qDebug() << "Atrás";
    emit movement(protocolo::atras);
}

void control_manual::buttonRight(){
    qDebug() << "Derecha";
    emit movement(protocolo::derecha);
}

void control_manual::buttonLeft(){
    qDebug() << "Izquierda";
    emit movement(protocolo::izquierda);
}

void control_manual::buttonInfra(){
    active_options[OPC_INFRA] = !active_options[OPC_INFRA];
    char img = !active_options[OPC_INFRA]?'1':'2';
    options_buttons[OPC_INFRA]->setIcon(QIcon(":/images/Imagenes/I" + QString(img) + ".png"));
    emit exc(protocolo::sensor_infrarojo, active_options[OPC_INFRA]);
}

void control_manual::buttonUltra(){
    active_options[OPC_ULTRA] = !active_options[OPC_ULTRA];
    char img = !active_options[OPC_ULTRA]?'1':'2';
    options_buttons[OPC_ULTRA]->setIcon(QIcon(":/images/Imagenes/U" + QString(img) + ".png"));
    emit exc(protocolo::sensor_distancia, active_options[OPC_ULTRA]);
}
void control_manual::buttonSearch(){
    active_options[OPC_SEARCH] = !active_options[OPC_SEARCH];
    char img = !active_options[OPC_SEARCH]?'1':'2';
    options_buttons[OPC_SEARCH]->setIcon(QIcon(":/images/Imagenes/B" + QString(img) + ".png"));
    emit find();
}
void control_manual::changeBehave(){
    QChar comportamiento = behaviors->currentData().toChar();
    for (int index = 0; index < n_buttons_options; index++)
        options_buttons[index]->setEnabled((comportamiento.toLatin1() == protocolo::Seguir_instruccion || index == OPC_SEARCH)?true:false);
    for (int index = 0; index < n_arrows; index++)
        arrows_buttons[index]->setEnabled((comportamiento.toLatin1() == protocolo::Seguir_instruccion)?true:false);
    qDebug() << comportamiento;
    emit behave(comportamiento.toLatin1());
}

void control_manual::updateAxis(int n, short v){
if(this->isActiveWindow() && arrows_buttons[protocolo::up]->isEnabled()){
    int c = 0;
    c = (v > 0)?1:2;
    int select = -1;
    if((c == 1 && v > protocolo::rango) || (c == 2 && v*(-1) > protocolo::rango))
        switch(n){
            case protocolo::eje_1:
                select = (c == 1)?protocolo::right:protocolo::left;
                break;
            case protocolo::eje_2:
                select = (c == 1)?protocolo::down:protocolo::up;
                break;
        }

    if(select >= 0){
        emit arrows_buttons[select]->clicked();
        arrows_buttons[select]->animateClick();
        arrows_buttons[select]->setFocus();
        //qDebug() << this->robot_name << " " << select;
    }
}
}

void control_manual::updateButton(int n, bool v){
    if(v && this->isActiveWindow())
        //qDebug() << this->robot_name << " " << n;
        switch(n){
            case 0:
                behaviors->setCurrentIndex(behaviors->findData(protocolo::Evadir));
            break;
            case 1:
                behaviors->setCurrentIndex(behaviors->findData(protocolo::Explorar));
            break;
            case 2:
                behaviors->setCurrentIndex(behaviors->findData(protocolo::Seguir_instruccion));
            break;
            case 3:
                behaviors->setCurrentIndex(behaviors->findData(protocolo::Detener));
            break;
            case 4:
                options_buttons[OPC_ULTRA]->animateClick();
            break;
            case 5:                
                options_buttons[OPC_INFRA]->animateClick();
            break;
            case 6:

            break;

            case 7:

            break;
        }
}

void control_manual::keyPressEvent(QKeyEvent *e){

    switch (e->key()) {
        case Qt::Key_Up:
            if(behaviors->currentData().toChar() == protocolo::Evadir)
                 behaviors->setCurrentIndex(behaviors->findData(protocolo::Seguir_instruccion));
            arrows_buttons[protocolo::up]->animateClick();
        break;
        case Qt::Key_Down:
            if(behaviors->currentData().toChar() == protocolo::Detener)
                 behaviors->setCurrentIndex(behaviors->findData(protocolo::Seguir_instruccion));
            arrows_buttons[protocolo::down]->animateClick();
        break;
        case Qt::Key_Right:
            arrows_buttons[protocolo::right]->animateClick();
        break;
        case Qt::Key_Left:
            arrows_buttons[protocolo::left]->animateClick();
        break;
        case Qt::Key_I:
            options_buttons[OPC_INFRA]->animateClick();
        break;
        case Qt::Key_U:
            options_buttons[OPC_ULTRA]->animateClick();
        break;
        case Qt::Key_Tab:
            behaviors->setFocusPolicy(Qt::NoFocus);
        break;
        case Qt::Key_Escape:
            this->close();
        break;
        default:
            behaviors->setFocusPolicy(Qt::NoFocus);
        break;
    }
}
control_manual::~control_manual(){
    delete joystick;
}

