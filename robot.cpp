#include "robot.h"
/*TODO: Una guía del protocolo*/
robot::robot(QString name, char identificator, char default_behave, QQueue<QString>* messages_queue, bool *queue_safe, QWidget *parent) : QGroupBox(name, parent){
    board = new Console(Qt::lightGray, Qt::black, "> ", false, this);
    this->setStyleSheet("QGroupBox{ background: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0, stop:0 #006666, stop: 1 #FFFFFF); color: black; font: 10pt ;font: bold; text-align:center;   } QGroupBox::title{ subcontrol-origin: margin;  subcontrol-position: top left;}");
    this->name = name;
    this->identificator = identificator;
    this->behave = new char[n_behaves];
    this->sender_safe = sender_safe;
    this->queue_safe = queue_safe;
    this->messages_queue = messages_queue;
    /*Nota el comportamiento por defecto quedará definido cuando se envie por primera vez*/
    this->behave[_main] = none;
    this->behave[secondary] = none;

    sensores = new QCheckBox*[protocolo::numero_excepciones] ;
    sensores[protocolo::sensor_distancia]   = new QCheckBox("Distancia", this);
    sensores[protocolo::sensor_infrarojo]   = new QCheckBox("Infrarojo", this);

    exceptions                              = new bool[protocolo::numero_excepciones];
    exceptions[protocolo::sensor_distancia] = false;
    exceptions[protocolo::sensor_infrarojo] = false;
    things_layout                           = new QVBoxLayout;
    actual_behavior                         = new QLabel("Comportamiento", this);
    exceptions_group                        = new QGroupBox("Exepciones", this);

    things_layout->addSpacing(2);
    exceptions_group->setStyleSheet(" QGroupBox{ background-color:transparent; color: black; font: 10pt ;font: bold; text-align:center;}");
    sensores[protocolo::sensor_infrarojo]->setStyleSheet("QCheckBox{color: black; font: 10pt}"); //Para que se siga viendo oscuro después de desabilitar
    sensores[protocolo::sensor_distancia]->setStyleSheet("QCheckBox{color: black; font: 10pt}");
    actual_behavior->setStyleSheet("QLabel{color:black; font:12pt; font:bold;}");
    exceptions_group->setStyleSheet("QGroupBox {background: transparent; border: none;}");
    exceptions_group->setEnabled(false);
    QVBoxLayout *exceptions_layout = new QVBoxLayout;
    exceptions_layout->addWidget(sensores[protocolo::sensor_infrarojo]);
    exceptions_layout->addWidget(sensores[protocolo::sensor_distancia]);
    exceptions_group->setLayout(exceptions_layout);
    things_layout->addWidget(board);
    things_layout->addWidget(exceptions_group);
    things_layout->addWidget(actual_behavior);
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
void robot::processOrder(QString data){
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
                    sensores[data.at(5).toLatin1() - '0']->setChecked(exceptions[data.at(5).toLatin1() - '0']);
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
void robot::setBehave(string new_behave){

}
void robot::operator<<(QString behave){
    qDebug() << behave;
}
void robot::operator<<(int behave){
    qDebug() << behave;
}
bool robot::setException(int exception_type, bool option){
    qDebug() << this->behave[_main] ;
    if(this->behave[_main] == protocolo::Seguir_instruccion
       && (exception_type == protocolo::sensor_distancia || exception_type == protocolo::sensor_infrarojo)){
        string message;
        message = protocolo::delimitador_i;
        message += this->identificator;
        message += protocolo::separador;
        message += protocolo::excepcion;
        message += protocolo::separador;
        message += exception_type + 48;
        message += protocolo::separador;
        message += option?'1':'0';
        message += protocolo::delimitador_f;
        *queue_safe = false;
        messages_queue->enqueue(QString().fromStdString(message));
        *queue_safe = true;
        return true;
    }else
        qDebug() << "Error[5]: No se pueden cambiar las excepeciones porque el comportamiento por defecto no es seguir instrucciones.";
    return false;
}

robot::~robot(){
    delete board;
    delete exceptions;
}

