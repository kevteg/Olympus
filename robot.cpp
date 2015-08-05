#include "robot.h"
/*TODO: Una guía del protocolo*/
robot::robot(QString name, char identificator, char default_behave, QQueue<QString>* messages_queue, bool *queue_safe, QWidget *parent) : QGroupBox("", parent){
    QFont f( "Adec", 14, QFont::Normal);
    board                                   = new Console(Qt::white, Qt::black, "> ", false, this);
    robot_nombre                            = new QLabel(this);
    this->identificator                     = identificator;
    this->behave                            = new char[n_behaves];
    this->sender_safe                       = sender_safe;
    this->queue_safe                        = queue_safe;
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


    this->setStyleSheet("QGroupBox{ background: rgb(27, 188, 155);} ");
    this->setFixedWidth(370);
    this->setFixedHeight(200);
    /*Nombre del robot*/
    robot_nombre->setText(name.toUpper());
    robot_nombre->setFont(f);
    robot_nombre->setStyleSheet("QLabel {color : rgb(44, 62, 80); }");
    robot_nombre->setAlignment(Qt::AlignCenter);

    actual_behavior->setFont(f);
    actual_behavior->setStyleSheet("QLabel {color : rgb(200, 247, 197); }");
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
                                          background-color: rgb(67, 97, 102);\
                                          }");
    control_layout->addWidget(control_manual_boton);
    control_layout->addWidget(actual_behavior);
    half_layout->addLayout(control_layout);
    half_layout->addWidget(board);
    things_layout->addLayout(half_layout);
    /*Sensors layout*/
    sensors[protocolo::sensor_distancia]->setText("DISTANCIA");
    sensors[protocolo::sensor_distancia]->setFont(f);
    sensors[protocolo::sensor_distancia]->setStyleSheet("QLabel {color : rgb(44, 62, 80); }");
    sensors[protocolo::sensor_distancia]->setAlignment(Qt::AlignCenter);
    sensors[protocolo::sensor_infrarojo]->setText("INFRAROJO");
    sensors[protocolo::sensor_infrarojo]->setFont(f);
    sensors[protocolo::sensor_infrarojo]->setStyleSheet("QLabel {color : rgb(44, 62, 80); }");
    sensors[protocolo::sensor_infrarojo]->setAlignment(Qt::AlignCenter);
    sensors_layout->addWidget(sensors[protocolo::sensor_distancia]);
    sensors_layout->addWidget(sensors[protocolo::sensor_infrarojo]);
    things_layout->addLayout(sensors_layout);

    /*Nota el comportamiento por defecto quedará definido cuando se envie por primera vez*/
    this->behave[_main] = none;
    this->behave[secondary] = none;
    this->setLayout(things_layout);

    string _first_m;
    /*TODO: Si el comportamiento tiene una parte secundaria*/
    _first_m = protocolo::delimitador_i;
    _first_m += this->identificator;
    _first_m += protocolo::separador;
    _first_m += default_behave;
    _first_m += protocolo::delimitador_f;
    *queue_safe = false;
    messages_queue->enqueue(QString().fromStdString(_first_m));
    *queue_safe = true;
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
        qDebug() << "Procesando: " << data;
        old_m = data;
        /*El robot vuelve a verificar que la orden este bien*/
        if(protocolo::verificacion(data.toLatin1(), 0) && data.at(1) == this->identificator){
            switch (protocolo::getTipoInstruccion(data.at(3).toLatin1())) {
            case protocolo::Comportamiento_tipo:
                if(behave[_main] != data.at(3).toLatin1()){
                    behave[_main] = data.at(3).toLatin1();
                    actual_behavior->setText("Comportamiento: " + protocolo::getCadenaInstruccion(data.at(3).toLatin1()) + "\n");
                    board->putData(QString("Nuevo comportamiento definido: " + protocolo::getCadenaInstruccion(data.at(3).toLatin1()) + "\n").toLatin1());
                }
                if(data.size() > protocolo::tam_min){
                    if(behave[secondary] != data.at(5).toLatin1()){
                        behave[secondary] = data.at(5).toLatin1();
                        board->putData(QString(QString::fromLatin1("Nueva dirección: ") + protocolo::getCadenaInstruccion(data.at(5).toLatin1()) + "\n").toLatin1());
                    }
                }else
                    behave[secondary] = none;
                if(data.at(3).toLatin1() == protocolo::Detener)
                    actual_behavior->setStyleSheet("QLabel{color:red; font:12pt; font:bold;}");
                else
                    actual_behavior->setStyleSheet("QLabel{color:black; font:12pt; font:bold;}");
            break;
            case protocolo::Busqueda_tipo:
                /*Podría ser que se guarde si se habia buscado*/
                board->putData(QString("Buscando...\n").toLatin1());
            break;
            case protocolo::Excepcion_tipo:
                if(data.size() > 7){
                    exceptions[data.at(5).toLatin1() - '0'] = data.at(7).toLatin1() - '0';
                    //sensors[data.at(5).toLatin1() - '0']->setChecked(exceptions[data.at(5).toLatin1() - '0']);
                    board->putData(QString(QString::fromLatin1("Excepción sensor ") + QString((!(data.at(5).toLatin1() - '0'))?"distancia":"infrarojo") + QString(exceptions[data.at(5).toLatin1() - '0']?" Activada":" Desactivada") + "\n").toLatin1());
                }
            break;
            }

        }else
            board->putData(QString(QString::fromLatin1("Error[4]: he recibido información errónea: ") + data.toLatin1() + "\n").toLatin1());
    }else
        qDebug() << "Mensaje ya procesado: " << data;
}
bool robot::getException(int exception_tipe){
    return exceptions[exception_tipe];
}
bool robot::changeOptions(char behave){
    string message;
    message = protocolo::delimitador_i;
    message += this->identificator;
    message += protocolo::separador;
    switch(behave){
        case protocolo::Explorar:
            message += protocolo::Explorar;
        break;
        case protocolo::Evadir:
            message += protocolo::Evadir;
        break;
        case protocolo::Seguir_instruccion:
            message += protocolo::Evadir;
        break;
        case protocolo::Detener:
            message += protocolo::Detener;
        break;
        case protocolo::buscar:
            message += protocolo::buscar;
        break;
        default:
            qDebug() << "Error[6]: Opción errónea. No se ha cambiado nada";
            return false;
        break;
    }
    message = protocolo::delimitador_f;
    *queue_safe = false;
    messages_queue->enqueue(QString().fromStdString(message));
    *queue_safe = true;
    return true;
}
bool robot::changeOptions(char behave, char option_1){
    string message;
    message = protocolo::delimitador_i;
    message += this->identificator;
    message += protocolo::separador;
    switch(behave){
        case protocolo::Seguir_instruccion:
            message += protocolo::Evadir;
            message += protocolo::separador;
            switch(option_1){
                case protocolo::adelante:
                    message += protocolo::adelante;
                break;
                case protocolo::atras:
                    message += protocolo::atras;
                break;
                case protocolo::derecha:
                    message += protocolo::derecha;
                break;
                case protocolo::izquierda:
                    message += protocolo::izquierda;
                break;
                default:
                    qDebug() << "Error[7]: Opción errónea. Seguir instrucciones no posee esa instrucción";
                    return false;
                break;
            }
        break;
        default:
            qDebug() << "Error[8]: Opción errónea. Seguir instrucciones es el único comportamiento con mas opciones";
            return false;
        break;
    }
    message = protocolo::delimitador_f;
    *queue_safe = false;
    messages_queue->enqueue(QString().fromStdString(message));
    *queue_safe = true;
    return true;
}
bool robot::changeOptions(int exception_type, bool option){
    qDebug() << this->behave[_main] ;
    if(this->behave[_main] == protocolo::Seguir_instruccion
       && (exception_type == protocolo::sensor_distancia || exception_type == protocolo::sensor_infrarojo)){
        string message;
        message = protocolo::delimitador_i;
        message += this->identificator;
        message += protocolo::separador;
        message += protocolo::excepcion;
        message += protocolo::separador;
        message += exception_type + 48; //Para llevarlo a char
        message += protocolo::separador;
        message += option?'1':'0';
        message += protocolo::delimitador_f;
        *queue_safe = false;
        messages_queue->enqueue(QString().fromStdString(message));
        *queue_safe = true;
        return true;
    }else
        qDebug() << "Error[5]: No se pueden cambiar las excepciones porque el comportamiento por defecto no es seguir instrucciones.";
    return false;
}

robot::~robot(){
    delete board;
    delete exceptions;
}

