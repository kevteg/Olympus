#include "robot.h"
/*TODO: Una guía del protocolo*/
robot::robot(QString name, char identificator, char default_behave, QQueue<QString>* messages_queue, bool *queue_safe, QWidget *parent) : QGroupBox(name, parent){
    board = new Console(Qt::lightGray, Qt::black, "> ", this);
    this->setStyleSheet("QGroupBox{ background: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0, stop:0 #006666, stop: 1 #FFFFFF); color: black; font: 10pt ;font: bold; text-align:center;   } QGroupBox::title{ subcontrol-origin: margin;  subcontrol-position: top left;}");
    this->name = name;
    this->identificator = identificator;
    this->behave = default_behave;

    exceptions = new bool[protocolo::numero_excepciones];
    exceptions[protocolo::sensor_distancia] = false;
    exceptions[protocolo::sensor_infrarojo] = false;
    //qDebug() << QChar(default_behave);
    things_layout = new QVBoxLayout;
    actual_behavior = new QLabel("Comportamiento", this);
    exceptions_group = new QGroupBox("Exepciones", this);
    sensor_infrarojo = new QCheckBox("Infrarojo", this);
    sensor_ultrasonido = new QCheckBox("Ultrasonido", this);
    things_layout->addSpacing(2);
    exceptions_group->setStyleSheet(" QGroupBox{ background-color:transparent; color: black; font: 10pt ;font: bold; text-align:center;}");
    sensor_infrarojo->setStyleSheet("QCheckBox{color: black; font: 10pt}"); //Para que se siga viendo oscuro después de desabilitar
    sensor_ultrasonido->setStyleSheet("QCheckBox{color: black; font: 10pt}");
    actual_behavior->setStyleSheet("QLabel{color:black; font:12pt; font:bold;}");
    exceptions_group->setStyleSheet("QGroupBox {background: transparent; border: none;}");
    exceptions_group->setEnabled(false);
    QVBoxLayout *exceptions_layout = new QVBoxLayout;
    exceptions_layout->addWidget(sensor_infrarojo);
    exceptions_layout->addWidget(sensor_ultrasonido);
    exceptions_group->setLayout(exceptions_layout);
    things_layout->addWidget(board);
    things_layout->addWidget(exceptions_group);
    things_layout->addWidget(actual_behavior);
    this->setLayout(things_layout);
    string _first_m;
    _first_m = protocolo::delimitador_i;
    _first_m += this->identificator;
    _first_m += protocolo::separador;
    _first_m += this->behave;
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
    qDebug() << "Procesando: " << data;
    /*El robot vuelve a verificar que la orden este bien*/
    if(protocolo::verificacion(data.toLatin1(), 0) && data.at(1) == this->identificator){
        switch(data.at(3).toLatin1()){
            case protocolo::Explorar:
                behave = protocolo::Explorar;
                actual_behavior->setStyleSheet("QLabel{color:black; font:12pt; font:bold;}");
                actual_behavior->setText("Comportamiento: " + protocolo::getCadenaComportamiento(protocolo::Explorar) + "\n");
                board->putData(QString("Nuevo comportamiento definido: " + protocolo::getCadenaComportamiento(protocolo::Explorar) + "\n").toLatin1());
            break;
            case protocolo::Evadir:
                behave = protocolo::Evadir;
                actual_behavior->setStyleSheet("QLabel{color:black; font:12pt; font:bold;}");
                actual_behavior->setText("Comportamiento: " + protocolo::getCadenaComportamiento(protocolo::Evadir) + "\n");
                board->putData(QString("Nuevo comportamiento definido: " + protocolo::getCadenaComportamiento(protocolo::Evadir) + "\n").toLatin1());
            break;
            case protocolo::Seguir_instruccion:
                behave = protocolo::Seguir_instruccion;
                actual_behavior->setText("Comportamiento: " + protocolo::getCadenaComportamiento(protocolo::Seguir_instruccion) + "\n");
                actual_behavior->setStyleSheet("QLabel{color:black; font:12pt; font:bold;}");
                board->putData(QString("Nuevo comportamiento definido: " + protocolo::getCadenaComportamiento(protocolo::Seguir_instruccion) + "\n").toLatin1());
            break;
            case protocolo::Detener:
                behave = protocolo::Detener;
                actual_behavior->setStyleSheet("QLabel{color:red; font:12pt; font:bold;}");
                actual_behavior->setText("Comportamiento: " + protocolo::getCadenaComportamiento(protocolo::Detener) + "\n");
                board->putData(QString("Nuevo comportamiento definido: " + protocolo::getCadenaComportamiento(protocolo::Detener) + "\n").toLatin1());
            break;
        }

    }else
        board->putData("Error: he recibido información errónea: "+ data.toLatin1()+"\n");
}
bool robot::getException(int exception_tipe){
    return exceptions[exception_tipe];
}
void robot::setBehave(string new_behave){

}
robot::~robot(){
    delete board;
    delete exceptions;
}

