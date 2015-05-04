#include "olymain.h"
#include "ui_olymain.h"

olymain::olymain(QWidget *parent) : QMainWindow(parent), ui(new Ui::olymain){
    ui->setupUi(this);
    int x = 0, y = 0;
    terminal        = new Console(Qt::black, Qt::white, ">> ", this);
    board           = new Console(Qt::lightGray, Qt::black, "> ", this);
    options         = new QToolButton*[n_options];
    string *botones = new string[n_options];
    settings        = new preferencias(this);
    serial          = new QSerialPort(this);
    sender          = new messenger(serial);
    swarm_object    = new swarm();
    //Cola de mensajes que se irán guardando y enviando cada cierto tiempo
    messages_queue  = new QQueue<QString>;

    sender_safe     = true;
    queue_safe      = true;

    botones[option_connect]        = string("Conectar");
    botones[option_disconnect]     = string("Desconectar");
    botones[option_stop]           = string("Detener");
    botones[option_start]          = string("Iniciar");
    botones[option_manual_control] = string("Control manual");
    botones[option_exit]           = string("Salir");
    messages_queue->enqueue("<A:A>");
    messages_queue->enqueue("<B:A>");
    messages_queue->enqueue("<C:A>");
    messages_queue->enqueue("<A:A>");
    messages_queue->enqueue("<B:A>");
    messages_queue->enqueue("<asdlajsdh>");
    messages_queue->enqueue("<a>");



    qDebug()<<"Hilo principal: "<<QThread::currentThreadId();
    checker = new verifyTime(terminal, swarm_object, &sender_safe, &queue_safe, sender, messages_queue, this);
    connect(&timer, SIGNAL(timeout()), checker, SLOT(onTimeout()));
    timer.start(500);
    timer.moveToThread(&thread);
    checker->moveToThread(&thread);
    /*EL HILO SE DEBE INICIAR DESPUÉS DE REALIZAR LA CONEXIÓN REAL. POR AHORA ES SOLO UNA PRUEBA*/


    QGroupBox *terminal_container        = new QGroupBox("Terminal", this);
    QGroupBox *board_container           = new QGroupBox("Pizarrón", this);
    QGroupBox *options_container         = new QGroupBox("Opciones", this);
    QVBoxLayout *terminal_layout         = new QVBoxLayout();
    QVBoxLayout *board_layout            = new QVBoxLayout();
    QGridLayout *internal_options_layout = new QGridLayout();

    if(!openPreFile()){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Archivo de configuración no encontrado!");
        messageBox.setFixedSize(500,200);
        exit(-1);
    }else{
        terminal_layout->addWidget(terminal);
        board_layout->addWidget(board);
        terminal_container->setLayout(terminal_layout);
        board_container->setLayout(board_layout);
        options_container->setLayout(internal_options_layout);

        ui->robots_container->setStyleSheet("QGroupBox{ background: rgb(159, 159, 159); color: black; font: 11pt ;font: bold;   } QGroupBox::title{ subcontrol-origin: margin;  subcontrol-position: top center;} QGroupBox { margin-top: 7px; border: 2px solid black; border-radius: 3px; }");
        ui->controls_container->setStyleSheet("QGroupBox{ background: rgb(159, 159, 159);color: black; font: 11pt ;font: bold;   } QGroupBox::title{ subcontrol-origin: margin;  subcontrol-position: top center;} QGroupBox { margin-top: 7px; border: 2px solid black; border-radius: 3px; }");
        ui->control_layout->setDirection(QBoxLayout::RightToLeft);
        ui->control_layout->addWidget(board_container);
        ui->control_layout->addWidget(terminal_container);
        ui->options_layout->addWidget(options_container);
        /*Para agregar las opciones de manera dinámica*/
        for (int index = 0; index < n_options; index++) {
            stringstream numero;
            if(index > 2){
                x = 1;
                y = index - 3;
            }
            numero << index;
            string dir = string(":/images/Imagenes/") + numero.str() + string(".png");
            options[index] = new QToolButton(this);
            options[index]->setIcon(QIcon(QString::fromStdString(dir)));
            options[index]->setIconSize(QSize(100, 100));
            options[index]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            options[index]->setText(QString::fromStdString(botones[index]));
            internal_options_layout->addWidget(options[index], x, (index > 2)?y:index);
        }
        setConections();
        defaultSituation();
    }
    thread.start();
}
bool olymain::openPreFile(){
   /*Abrir el archivo de configuración crear los robots y agregarlos al layout*/
   string line;
   int index = 0;
    ifstream pre("conf.oly");
    if(pre.is_open()){
        while(getline(pre, line)){
            if(line[0] == '>'){
                QString nombre = QString::fromStdString(line.substr(3));
                swarm_object->getRobots()->push_back(new robot(nombre, line[1], line[2], messages_queue, &queue_safe, this));
                ui->robots_layout->addWidget(swarm_object->getRobots()->at(index++));
            }else if(line[0] == '<'){
                size_t pos = line.find(":");
                QString des = QString::fromStdString(line.substr(pos + 1));
                manual_control = (des == "si")?true:false;
            }
        }
    }else
       return false;
    return true;
}
void olymain::defaultSituation(){
    options[option_disconnect]->setEnabled(false);
    options[option_start]->setEnabled(false);
    options[option_stop]->setEnabled(false);
    options[option_manual_control]->setEnabled(false);
    terminal->putData("Bienvenido a Olympus");
    terminal->putData("\n");
}
void olymain::setConections(){
    /*Opciones*/
    connect(options[option_exit], SIGNAL(clicked()), this, SLOT(close()));
    connect(options[option_connect], SIGNAL(clicked()), this, SLOT(connection()));
    connect(options[option_disconnect], SIGNAL(clicked()), this, SLOT(disconnection()));
    connect(options[option_start], SIGNAL(clicked()), this, SLOT(begin()));
    connect(options[option_stop], SIGNAL(clicked()), this, SLOT(stop()));
    /*Preferencias*/
    connect(ui->actionAcerca_de_Olympus, SIGNAL(triggered()), this, SLOT(openAbout()));
    connect(ui->actionPreferencias, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionSalir, SIGNAL(triggered()), this, SLOT(close()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(recieveInformation()));
}
void olymain::connection(){
     messages_queue->enqueue("<A:A>");
     /*AQUI DEBERIA INICIAR EL HILO*/
    /*preferencias::preferencia opt = settings->Preferencia();
    if(sender->createConnection(opt)){
        options[option_connect]->setEnabled(false);
        options[option_disconnect]->setEnabled(true);
        options[option_start]->setEnabled(true);
        ui->statusBar->showMessage(tr("Conectado correctamente a: %1 : %2").arg(opt.nombre).arg(opt.stringBaudRate));
    }else{
        QMessageBox::critical(this, tr("Error con el puerto seleccionado"), serial->errorString());
        ui->statusBar->showMessage(tr("Error al abrir serial"));
    }*/
}
void olymain::disconnection(){
    sender->breakConnection();
    options[option_connect]->setEnabled(true);
    options[option_disconnect]->setEnabled(false);
    options[option_manual_control]->setEnabled(false);
    options[option_start]->setEnabled(false);
    options[option_stop]->setEnabled(false);
}
void olymain::begin(){
    options[option_manual_control]->setEnabled(manual_control);
    options[option_start]->setEnabled(false);
    options[option_stop]->setEnabled(true);
}
void olymain::stop(){
    options[option_manual_control]->setEnabled(false);
    options[option_start]->setEnabled(true);
    options[option_stop]->setEnabled(false);
}
void olymain::openAbout(){
    QMessageBox::about(this, tr("Acerca de Olympus 3"),
                           tr("Programado por Kevin Hernández"
                              "\nUniversidad Nacional Experimental del Táchira - Laboratorio de prototipos"));
}
void olymain::recieveInformation(){
   if(sender_safe){
       sender_safe = false;
       QString robot_name;
       QByteArray data = serial->readAll();
       robot_name = swarm_object->sendData(data);
       if(robot_name != "err")
           terminal->putData(QString("Se recibe: " + QString(data) + " de " + robot_name + "\n").toLatin1());
       else
           terminal->putData(QString("Mensaje: " + QString(data) + " está dañado o no cumple con el protocolo\n").toLatin1());
       sender_safe = true;
   }
}

olymain::~olymain(){
    thread.quit();
    timer.stop();
    delete ui;
    delete board;
    delete terminal;
    delete sender;
    for(int index = 0; index < n_options; index++)
        delete options[index];
    delete settings;
    delete messages_queue;
    delete checker;
    delete swarm_object;
}
