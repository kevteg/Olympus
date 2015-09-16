#include "robot.h"
/*TODO: Una guía del protocolo*/
robot::robot(QString name, char identificator, char default_behave, QQueue<QString>* messages_queue, QWidget *parent) : QGroupBox("", parent){
    QFont f( "Adec", 14, QFont::Normal);
    speed                                   = protocolo::default_speed;
    this->name                              = name;
    board                                   = new Console(Qt::white, Qt::black, "> ", false, this);
    robot_nombre                            = new QLabel(this);
    this->identificator                     = identificator;
    this->behave                            = new char[n_behaves];
    this->messages_queue                    = messages_queue;
    things_layout                           = new QVBoxLayout;
    half_layout                             = new QHBoxLayout;
    sensors_layout                          = new QHBoxLayout;
    control_layout                          = new QVBoxLayout;
    control_manual_boton                    = new QToolButton(this);
    actual_behavior                         = new QLabel("ESPERANDO", this);
    sensors                                 = new QLabel*[protocolo::numero_excepciones];
    sensors[protocolo::sensor_distancia]    = new QLabel(this);
    sensors[protocolo::sensor_infrarojo]    = new QLabel(this);
    exceptions                              = new bool[protocolo::numero_excepciones];
    exceptions[protocolo::sensor_distancia] = false;
    exceptions[protocolo::sensor_infrarojo] = false;
    control                                 = new control_manual(name, speed);
    control_manual_boton->setEnabled(false);

    this->setStyleSheet("QGroupBox{ background: rgb(27, 188, 155);} ");
    this->setFixedWidth(370);
    this->setFixedHeight(200);
    /*Nombre del robot*/
    robot_nombre->setText(name.toUpper());
    robot_nombre->setFont(f);
    robot_nombre->setStyleSheet(AZUL);
    robot_nombre->setAlignment(Qt::AlignCenter);

    actual_behavior->setFont(f);
    actual_behavior->setStyleSheet(BLANCO);
    actual_behavior->setAlignment(Qt::AlignCenter);
    things_layout->addWidget(robot_nombre);

    /*Half layout, control, comportamiento y consola*/
    /*Vertical layout: Control y comportamiento*/
    control_manual_boton->setIcon(QIcon(QString::fromStdString(":/images/Imagenes/5.png")));
    control_manual_boton->setIconSize(QSize(100, 100));
    control_manual_boton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    control_manual_boton->setStyleSheet("QToolButton {\
                                          border-radius: 50%;\
                                          width: 100px;\
                                          height: 100px;\
                                          position: centered;} \
                                          QToolButton:pressed { \
                                          background-color: rgb(33, 193, 163);\
                                          }");
    control_layout->addWidget(control_manual_boton);
    control_layout->addWidget(actual_behavior);
    half_layout->addLayout(control_layout);
    half_layout->addWidget(board);
    things_layout->addLayout(half_layout);
    /*Sensors layout*/
    sensors[protocolo::sensor_distancia]->setText("DISTANCIA");
    sensors[protocolo::sensor_distancia]->setFont(f);
    sensors[protocolo::sensor_distancia]->setStyleSheet(GRIS);
    sensors[protocolo::sensor_distancia]->setAlignment(Qt::AlignCenter);
    sensors[protocolo::sensor_infrarojo]->setText("INFRAROJO");
    sensors[protocolo::sensor_infrarojo]->setFont(f);
    sensors[protocolo::sensor_infrarojo]->setStyleSheet(GRIS);
    sensors[protocolo::sensor_infrarojo]->setAlignment(Qt::AlignCenter);
    sensors_layout->addWidget(sensors[protocolo::sensor_distancia]);
    sensors_layout->addWidget(sensors[protocolo::sensor_infrarojo]);
    things_layout->addLayout(sensors_layout);
    setConnections();
    /*Nota el comportamiento por defecto quedará definido cuando se envie por primera vez*/
    this->behave[_main] = none;
    this->behave[secondary] = none;
    this->setLayout(things_layout);

    setBehave(default_behave);
}

QString robot::getName(){
    return name;
}

char robot::getIdentificator(){
    return identificator;
}
void robot::operator<<(QString data){
    /*El operador se usa para procesar los mensajes que se van enviando*/
    if(data != old_m){
        //control_manual_boton->setEnabled(true);
        qDebug() << "Robot " << name << " procesando: " << data;
        old_m = data;
        /*El robot vuelve a verificar que la orden este bien*/
        if(protocolo::verificacion(data.toLatin1(), 0) && data.at(1) == this->identificator){
            switch (protocolo::getTipoInstruccion(data.at(3).toLatin1())) {
            case protocolo::Comportamiento_tipo:
                if(!(data.size() > protocolo::tam_min))
                    setBehave_(data.at(3).toLatin1());
                else
                    setBehave_(data.at(3).toLatin1(), data.at(5).toLatin1());
            break;
            case protocolo::Busqueda_tipo:
                /*Podría ser que se guarde si se habia buscado*/
                if(data.size() > protocolo::tam_min){
                    board->putData(QString("Robot envia:\n").toLatin1());

                    if(!(data.size() > 7)) //Si es mayor que siete significa que el robot responde el comportamiento más su opción
                        setBehave_(data.at(5).toLatin1());
                    else
                        setBehave_(data.at(5).toLatin1(), data.at(7).toLatin1());

                }else
                    board->putData(QString("Buscando...\n").toLatin1());
            break;
            case protocolo::Excepcion_tipo:
                if(data.size() > 7){
                    exceptions[data.at(5).toLatin1() - '0'] = data.at(7).toLatin1() - '0';
                    sensors[data.at(5).toLatin1() - '0']->setStyleSheet(!(data.at(7).toLatin1() - '0')?GRIS:ROJO);
                    board->putData(QString(QString::fromLatin1("Excepción sensor ") + QString((!(data.at(5).toLatin1() - '0'))?"distancia":"infrarojo") + QString(exceptions[data.at(5).toLatin1() - '0']?" Activada":" Desactivada") + "\n").toLatin1());
                }
            break;
            case protocolo::Velocidad_tipo:
                if(data.size() > 7 && ((data.at(5).toLatin1() - 48) == protocolo::sel_vel_1 ||
                                       (data.at(5).toLatin1() - 48) == protocolo::sel_vel_2)
                        && (data.at(7).toLatin1() - 48) >= protocolo::min_vel && (data.at(7).toLatin1() - 48) <= protocolo::max_vel){

                    speed = calculateSpeed((data.at(5).toLatin1() - 48), (data.at(7).toLatin1() - 48));
                    board->putData(QString("Velocidad del robot: " + QString::number(speed) + "\n").toLatin1());
                }
            break;
            }

        }else
            board->putData(QString(QString::fromLatin1("Error[4]: he recibido información errónea: ") + data.toLatin1() + "\n").toLatin1());
    }else
        qDebug() << "Mensaje ya procesado: " << data;
}
void robot::setConnections(){
    connect(control_manual_boton, SIGNAL(clicked()), this, SLOT(showControl()));
    connect(control, SIGNAL(movement(char)), this, SLOT(seguirInstrucciones(char)));
    connect(control, SIGNAL(exc(int, bool)), this, SLOT(setException(int, bool)));
    connect(control, SIGNAL(behave(char)), this, SLOT(setBehave(char)));
    connect(control, SIGNAL(find()), this, SLOT(find()));
    connect(control, SIGNAL(newVel(int,int)), this, SLOT(setSpeed(int, int)));
}

bool robot::getException(int exception_tipe){
    return exceptions[exception_tipe];
}
bool robot::setException(int exception_type, bool option){
    bool retorno = false;
    string message;
    //qDebug() << this->behave[_main] ;
    if(this->behave[_main] == protocolo::Seguir_instruccion
       && (exception_type == protocolo::sensor_distancia || exception_type == protocolo::sensor_infrarojo)){       
        retorno = true;
        message = protocolo::delimitador_i;
        message += this->identificator;
        message += protocolo::separador;
        message += protocolo::excepcion;
        message += protocolo::separador;
        message += exception_type + 48; //Para llevarlo a char
        message += protocolo::separador;
        message += option?'1':'0';
        message += protocolo::delimitador_f;
        protocolo::queue_safe = false;
        messages_queue->enqueue(QString().fromStdString(message));
        protocolo::queue_safe = true;
    }else
        qDebug() << "Error[5]: No se pueden cambiar las excepciones porque el comportamiento por defecto no es seguir instrucciones.";
    return retorno;
}

bool robot::setBehave(char behave, char opcion){
    bool retorno = false;
    string message;
    if(protocolo::getTipoInstruccion(behave) == protocolo::Comportamiento_tipo){
        retorno = true;
        message = protocolo::delimitador_i;
        message += this->identificator;
        message += protocolo::separador;
        message += behave;
        if(protocolo::getTipoInstruccion(opcion) == protocolo::opcion_cmp_tipo){
            message += protocolo::separador;
            message += opcion;
        }
        message += protocolo::delimitador_f;
        protocolo::queue_safe = false;
        messages_queue->enqueue(QString().fromStdString(message));
        protocolo::queue_safe = true;
    }else
        qDebug() << "Error[6.1]: Ese comportamiento es incorrecto";
    return retorno;
}
bool robot::setBehave(char behave){
    bool retorno = false;
    string message;
    if(protocolo::getTipoInstruccion(behave) == protocolo::Comportamiento_tipo){
        retorno = true;
        message = protocolo::delimitador_i;
        message += this->identificator;
        message += protocolo::separador;
        message += behave;
        message += protocolo::delimitador_f;
        protocolo::queue_safe = false;
        messages_queue->enqueue(QString().fromStdString(message));
        protocolo::queue_safe = true;
    }else
        qDebug() << "Error[6.2]: Ese comportamiento es incorrecto";
    return retorno;
}

bool robot::evadir(){
    return setBehave(protocolo::Evadir);
}

bool robot::detener(){
    return setBehave(protocolo::Detener);
}

bool robot::seguirInstrucciones(char opcion){
    return setBehave(protocolo::Seguir_instruccion, opcion);
}
bool robot::seguirInstrucciones(){
    return setBehave(protocolo::Seguir_instruccion);
}
bool robot::explorar(){
    return setBehave(protocolo::Explorar);
}

void robot::find(){
    string message;
    message = protocolo::delimitador_i;
    message += this->identificator;
    message += protocolo::separador;
    message += protocolo::buscar;
    message += protocolo::delimitador_f;
    qDebug() << QString().fromStdString(message);
    protocolo::queue_safe = false;
    messages_queue->enqueue(QString().fromStdString(message));
    protocolo::queue_safe = true;
}

control_manual* robot::getControl(){
    return control;
}
QToolButton* robot::getBotonControl(){
    return control_manual_boton;
}
void robot::setSpeed(int x, int y){
    string message;
    if((x == protocolo::sel_vel_1 || x == protocolo::sel_vel_2) && y >= protocolo::min_vel && y <= protocolo::max_vel){
        message = protocolo::delimitador_i;
        message += this->identificator;
        message += protocolo::separador;
        message += protocolo::velocidad;
        message += protocolo::separador;
        message += x + 48;
        message += protocolo::separador;
        message += y + 48;
        message += protocolo::delimitador_f;
        qDebug() << QString().fromStdString(message);
        protocolo::queue_safe = false;
        messages_queue->enqueue(QString().fromStdString(message));
        protocolo::queue_safe = true;
    }
}
int robot::calculateSpeed(int x, int y){
    return protocolo::min_vel_real + (y + x*10)*10;
}
void robot::setSpeed(int speed){
    int x, y;
    x = (speed > (protocolo::min_vel_real + protocolo::max_vel_real)/2)?protocolo::sel_vel_2:protocolo::sel_vel_1;
    y = ((speed - protocolo::min_vel_real)/10) - x*10;
    setSpeed(x, y);
}
void robot::setBehave_(char behave){
    if(this->behave[_main] != behave){
        this->behave[_main] = behave;
        actual_behavior->setText(protocolo::getCadenaInstruccion(behave).toUpper());
        if(actual_behavior->text().count() > 10){
            QFont fuente("Adec", 10, QFont::Normal);
            actual_behavior->setFont(fuente);
        }else{
            QFont fuente("Adec", 14, QFont::Normal);
            actual_behavior->setFont(fuente);
        }

        board->putData(QString("Comportamiento actual: " + protocolo::getCadenaInstruccion(behave) + "\n").toLatin1());
    }
    this->behave[secondary] = none;
    if(this->behave[_main] == protocolo::Detener)
        actual_behavior->setStyleSheet(ROJO);
    else
        actual_behavior->setStyleSheet(BLANCO);
}

void robot::setBehave_(char behave, char option){
    setBehave_(behave);
    if(this->behave[secondary] != option){
        this->behave[secondary] = option;
        if(this->behave[secondary] == protocolo::opcion_cmp_tipo)
            board->putData(QString(QString::fromLatin1("Dirección: ") + protocolo::getCadenaInstruccion(option) + "\n").toLatin1());
    }
}

void robot::showControl(){
    control->setCurr_robot_vel(speed);
    control->show();
}

robot::~robot(){
    delete board;
    delete exceptions;
    delete control;
}


