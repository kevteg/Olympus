#include "olymain.h"
#include "ui_olymain.h"

olymain::olymain(QWidget *parent) : QMainWindow(parent), ui(new Ui::olymain){
    ui->setupUi(this);
    terminal                             = new Console(Qt::black, Qt::white, ">> ", true, this);
    board                                = new Console(Qt::white, Qt::black, "> ", false, this);
    options                              = new QToolButton*[n_options];
    string *botones                      = new string[n_options];
    settings                             = new preferencias(this);
    serial                               = new QSerialPort(this);
    sender                               = new messenger(serial);
    swarm_object                         = new swarm();
    messages_queue                       = new QQueue<QString>;
    show_comming_info                    = true;
    sender_safe                          = true;
    queue_safe                           = true;
    var                                  = "";

    botones[option_connect_disconnect]   = string("Conectar/Desconectar");
    botones[option_start_stop]           = string("Iniciar/detener");
    botones[option_preferences]          = string("Preferencias");
    botones[option_about]                = string("Acerca de");
    botones[option_exit]                 = string("Salir");

    checker                              = new verifyTime(terminal, swarm_object, &sender_safe, &queue_safe, sender, messages_queue, this);
    QGroupBox *board_container           = new QGroupBox("", this);
    QGroupBox *terminal_container        = new QGroupBox("", this);
    QGroupBox *options_container         = new QGroupBox("", this);
    QGroupBox *info_container            = new QGroupBox("", this);
    serial_connection                    = false;
    rutine_robots                        = false;

    for (int i = 0; i < n_options; i++) {
        stringstream numero;
        numero << i;
        options[i] = new QToolButton(this);
        string dir = string(":/images/Imagenes/") + numero.str() + string(".png");
        options[i]->setIcon(QIcon(QString::fromStdString(dir)));

        if(i < 2){
            options[i]->setIconSize(QSize(100, 100));
            options[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
            options[i]->setStyleSheet("QToolButton {\
                                      border-radius: 50%;\
                                      width: 200px;\
                                      height: 100px;\
                                      position: centered;} \
                                      QToolButton:pressed { \
                                      background-color: rgb(67, 97, 102);\
            }");
        }else{
            options[i]->setIconSize(QSize(30, 30));
            options[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
            options[i]->setStyleSheet("QToolButton {\
                                      border-radius: 1%;\
                                      background: rgb(52, 77, 77);\
                                      width: 50px;\
                                      height: 50px;\
                                      position: centered;} \
                                      QToolButton:pressed { \
                                      background-color: rgb(68, 88, 90);\
            }");
        }
    }


    terminal_container->setStyleSheet("QGroupBox{\
                     background: rgb(212, 212, 212); }\
                    QGroupBox::title{ subcontrol-origin: margin;\
                    subcontrol-position: top center;} \
                    QGroupBox { margin-top: 7px; border: 0px \
                    solid black; border-radius: 3px; }");

    board_container->setStyleSheet("QGroupBox{\
                     background: rgb(62, 70, 81); } \
                    QGroupBox::title{ subcontrol-origin: margin;\
                    subcontrol-position: top center;} \
                    QGroupBox { margin-top: 7px; border: 0px \
                    solid black; border-radius: 3px; }");

   options_container->setStyleSheet("QGroupBox{\
                     background: rgb(58, 84, 88);  } \
                    QGroupBox::title{ subcontrol-origin: margin;\
                    subcontrol-position: top center;} \
                    QGroupBox { margin-top: 7px; border: 0px \
                    solid black; border-radius: 3px; }");
    info_container->setStyleSheet("QGroupBox{\
                      background: rgb(94, 108, 125);  } \
                     QGroupBox::title{ subcontrol-origin: margin;\
                     subcontrol-position: top center;} \
                     QGroupBox { margin-top: 7px; border: 0px \
                     solid black; border-radius: 3px; }");


    QFont f( "Adec", 14, QFont::Normal);

    QLabel *terminal_titulo = new QLabel(this);
    terminal_titulo->setText("CONSOLA");
    terminal_titulo->setFont(f);
    terminal_titulo->setStyleSheet("QLabel {color : rgb(44, 62, 80); }");
    terminal_titulo->setAlignment(Qt::AlignCenter);

    QLabel *pizarron_titulo = new QLabel(this);
    pizarron_titulo->setText("PIZARRÓN");
    pizarron_titulo->setFont(f);
    pizarron_titulo->setStyleSheet("QLabel {color : rgb(238, 238, 238); }");
    pizarron_titulo->setAlignment(Qt::AlignCenter);

    QLabel *opciones_titulos[3];
    for (int i = 0; i < 3; i++) {
        opciones_titulos[i] = new QLabel(this);
        opciones_titulos[i]->setFont(f);
        opciones_titulos[i]->setStyleSheet("QLabel {color: rgb(236, 240, 241); }");
        opciones_titulos[i]->setAlignment(Qt::AlignCenter);
    }
    opciones_titulos[0]->setText("OPCIONES");
    opciones_titulos[1]->setText("CONEXIÓN SERIAL");
    opciones_titulos[2]->setText("RUTINAS DE ROBOTS");

    QVBoxLayout *terminal_layout         = new QVBoxLayout();
    QVBoxLayout *board_layout            = new QVBoxLayout();
    QVBoxLayout *internal_options_layout = new QVBoxLayout();
    QHBoxLayout *more_options_layout     = new QHBoxLayout();

    qDebug() << "Hilo principal: " << QThread::currentThreadId();
    if(!openPreFile()){
        QMessageBox messageBox;
        messageBox.critical(0,"Error[1]","Archivo de configuración no encontrado!");
        messageBox.setFixedSize(500,200);
        exit(-1);
    }else{
        terminal_layout->addWidget(terminal_titulo);
        terminal_layout->addWidget(terminal);
        terminal_container->setLayout(terminal_layout);

        board_layout->addWidget(pizarron_titulo);
        board_layout->addWidget(board);
        board_container->setLayout(board_layout);

        internal_options_layout->setAlignment(Qt::AlignHCenter);
        internal_options_layout->addWidget(opciones_titulos[0]);
        internal_options_layout->addWidget(opciones_titulos[1]);
        internal_options_layout->addWidget(options[0]);
        internal_options_layout->addWidget(opciones_titulos[2]);
        internal_options_layout->addWidget(options[1]);

        more_options_layout->addWidget(options[2]);
        more_options_layout->addWidget(options[3]);
        more_options_layout->addWidget(options[4]);
        internal_options_layout->addLayout(more_options_layout);

        options_container->setLayout(internal_options_layout);

        ui->robots_container->setStyleSheet("QGroupBox{\
                                             background: rgb(239, 242, 242); \
                                            color: black; font: 11pt ;font: bold;   } \
                                            QGroupBox::title{ subcontrol-origin: margin;\
                                            subcontrol-position: top center;} \
                                            QGroupBox { margin-top: 7px; border: 0px \
                                            solid black; border-radius: 0px; }");

        ui->controls_container->setStyleSheet("QGroupBox{\
                                              background: rgb(239, 242, 242); \
                                             color: black; font: 11pt ;font: bold;   } \
                                             QGroupBox::title{ subcontrol-origin: margin;\
                                             subcontrol-position: top center;} \
                                             QGroupBox { margin-top: 7px; border: 0px \
                                             solid black; border-radius: 0px; }");

        ui->control_layout->setDirection(QVBoxLayout::TopToBottom);
        ui->control_layout->addWidget(terminal_container);
        ui->control_layout->addWidget(board_container);
        ui->info_Layout->addWidget(info_container);
        ui->options_layout->addWidget(options_container);
        setConections();
        defaultSituation();
    }
        QPalette p = ui->statusBar->palette();
        p.setColor( QPalette::Background, QColor(1, 152, 117));
        p.setColor(QPalette::Text, Qt::white);
        ui->statusBar->setPalette(p);
        ui->statusBar->setAutoFillBackground(true);

    /*string _first_m;
    _first_m = protocolo::delimitador_i;
    _first_m += 'A';
    _first_m += protocolo::separador;
    _first_m += protocolo::Explorar;
    _first_m += protocolo::delimitador_f;
    (*swarm_object->getRobots()->at(0)) << QString::fromStdString(_first_m);*/

    //swarm_object->sendData(QString::fromStdString(_first_m).toLatin1());
}
bool olymain::openPreFile(){
   /*Abrir el archivo de configuración crear los robots y agregarlos al layout*/
   string line;
   int index = 0;
    ifstream pre("conf.oly");
    if(pre.is_open()){
        while(getline(pre, line)){
            if(line[0] == protocolo::delimitador_f){
                QString nombre = QString::fromStdString(line.substr(5));
                swarm_object->getRobots()->push_back(new robot(nombre, line[1], line[3], messages_queue, &queue_safe, this));
                ui->robots_layout->addWidget(swarm_object->getRobots()->at(index++));
            }else if(line[0] == protocolo::delimitador_i){
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
    options[option_start_stop]->setEnabled(false);
    terminal->putData("Bienvenido a Olympus\n");
}
void olymain::setConections(){
    /*Hilo*/
    connect(&timer, SIGNAL(timeout()), checker, SLOT(onTimeout()));
    connect(&thread, SIGNAL(finished()), &timer, SLOT(stop()));
    /*Opciones*/
    connect(options[option_exit], SIGNAL(clicked()), this, SLOT(close()));
    connect(options[option_connect_disconnect], SIGNAL(clicked()), this, SLOT(connect_serial()));
    connect(options[option_start_stop], SIGNAL(clicked()), this, SLOT(robotRutine()));
    connect(options[option_preferences], SIGNAL(clicked()), settings, SLOT(show()));
    connect(options[option_about], SIGNAL(clicked()), this, SLOT(openAbout()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(recieveInformation()));
    /*Console*/
    connect(terminal, SIGNAL(showAbout()), this, SLOT(openAbout()));
    connect(terminal, SIGNAL(changeMstate()), this, SLOT(changeMessState()));
    connect(terminal, SIGNAL(exitProgram()), this, SLOT(close()));

}
void olymain::changeMessState(){
    show_comming_info = !show_comming_info;
}

void olymain::connect_serial(){
    serial_connection = !serial_connection;
    if(serial_connection){
        connection();
    }else
        disconnection();
}
void olymain::connection(){
    preferencias::preferencia opt = settings->Preferencia();
    if(sender->createConnection(opt)){
        /*Cambiar al icono de desconeccion*/
        string dir = string(":/images/Imagenes/00.png");
        options[option_connect_disconnect]->setIcon(QIcon(QString::fromStdString(dir)));
        options[option_start_stop]->setEnabled(true);
        ui->statusBar->showMessage(tr("Conectado correctamente a: %1 : %2").arg(opt.nombre).arg(opt.stringBaudRate));
    }else{
        terminal->putData(QString("Error[2]: Problemas con el puerto").toLatin1());
        QMessageBox::critical(this, tr("Error con el puerto seleccionado"), serial->errorString());
        ui->statusBar->showMessage(tr("Error al abrir serial"));
    }
}
void olymain::disconnection(){
    sender->breakConnection();
    /*Icono a conectar*/
    string dir = string(":/images/Imagenes/0.png");
    options[option_connect_disconnect]->setIcon(QIcon(QString::fromStdString(dir)));
    options[option_start_stop]->setEnabled(false);
}
void olymain::robotRutine(){
    rutine_robots = !rutine_robots;
    if(rutine_robots)
        begin();
    else
        stop();
}
void olymain::begin(){
    timer.start(500);
    timer.moveToThread(&thread);
    thread.start();
    //thread.signal
    string dir = string(":/images/Imagenes/11.png");
    options[option_start_stop]->setIcon(QIcon(QString::fromStdString(dir)));
}
void olymain::stop(){
    timer.stop();
    string dir = string(":/images/Imagenes/1.png");
    options[option_start_stop]->setIcon(QIcon(QString::fromStdString(dir)));
}
void olymain::openAbout(){
    QMessageBox::about(this, tr("Acerca de Olympus 3"),
                           tr("Programado por Kevin Hernández"
                              "\nUniversidad Nacional Experimental del Táchira - Laboratorio de prototipos"));
}
void olymain::recieveInformation(){
   if(sender_safe){
       sender_safe = false;
       int index_1, index_2;
       QString robot_name;
       QString rec = "";
       QByteArray data = serial->readAll();

       var += data;
       //qDebug() << var;
       if(var.contains(protocolo::delimitador_f, Qt::CaseInsensitive)){
            index_1 = var.size() - 1;
            while(index_1 > 0 && var.at(index_1) != protocolo::delimitador_f)
                index_1--;

            index_2 = 0;
            while(index_2 < var.size() && var.at(index_2) != protocolo::delimitador_i)
                index_2++;
            rec = var.mid(index_2, (index_1 - index_2 + 1));

            //qDebug() << "REC: " << rec;
            var = "";
       }

       if(protocolo::verificacion(rec.toLatin1(), 0)){
           robot_name = swarm_object->sendData(rec.toLatin1());
           if(show_comming_info){
               if(robot_name != "err")
                   terminal->putData(QString("Se recibe: " + QString(rec) + " de " + robot_name + "\n").toLatin1());
               else
                   terminal->putData(QString("Error[3]: Mensaje: " + QString(rec) + QString::fromLatin1(" está dañado, no cumple con el protocolo\n")).toLatin1());
           }
       }
       sender_safe = true;
   }
}

olymain::~olymain(){
    thread.quit();
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
