#include "preferencias.h"
#include "ui_preferencias.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>

QT_USE_NAMESPACE


preferencias::preferencias(QWidget *parent) : QDialog(parent), ui(new Ui::preferencias){
    ui->setupUi(this);

    intValidator = new QIntValidator(0, 4000000, this);//Un intervalo para selecciones

    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);

    connect(ui->applyButton, SIGNAL(clicked()),
            this, SLOT(aplicar()));
    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(infoPuerto(int)));
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(checkCustomBaudRatePolicy(int)));

    mostrar_parametrosPuertos();
    mostrar_informacionPuertos();
    actualizarPreferencia();
}

preferencias::~preferencias(){
    delete ui;
}

preferencias::preferencia preferencias::Preferencia() const{
    return actualPreferencia;
}

void preferencias::infoPuerto(int idx){
    if (idx != -1) {
        QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
        ui->descriptionLabel->setText(tr("Descripcion: %1").arg(list.at(1)));
        ui->manufacturerLabel->setText(tr("Fabricante: %1").arg(list.at(2)));
        ui->serialNumberLabel->setText(tr("Número serial: %1").arg(list.at(3)));
        ui->locationLabel->setText(tr("Localización: %1").arg(list.at(4)));
        ui->vidLabel->setText(tr("Identificador de vendedor: %1").arg(list.at(5)));
        ui->pidLabel->setText(tr("Identificador de producto: %1").arg(list.at(6)));
    }
}

void preferencias::aplicar(){
    actualizarPreferencia();
    hide();
}

void preferencias::checkCustomBaudRatePolicy(int idx){
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate) {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void preferencias::mostrar_parametrosPuertos(){
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(QStringLiteral("Selección"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(QStringLiteral("Ninguna"), QSerialPort::NoParity);
    ui->parityBox->addItem(QStringLiteral("Nivelar"), QSerialPort::EvenParity);
    ui->parityBox->addItem(QStringLiteral("Impar"), QSerialPort::OddParity);
    ui->parityBox->addItem(QStringLiteral("Marca"), QSerialPort::MarkParity);
    ui->parityBox->addItem(QStringLiteral("Espacio"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN //Si es windows
    ui->stopBitsBox->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(QStringLiteral("Ninguna"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);

    ui->behaviorList->addItem(QStringLiteral("Explorar"), QString("A"));
    ui->behaviorList->addItem(QStringLiteral("Evadir"), QString("B"));
    ui->behaviorList->addItem(QStringLiteral("Seguir instrucciones"), QString("C"));

}

void preferencias::mostrar_informacionPuertos(){
    ui->serialPortInfoListBox->clear();
    static const QString blankString = QObject::tr("S/P"); //Si no hay puertos
    QString descripcion;
    QString fabricante;
    QString serial;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) { //Para cada puerto disponible
        QStringList list;
        descripcion = info.description(); //Se usan estas variable por si esta vacia la descripción
        fabricante = info.manufacturer();
        serial = info.serialNumber();
        list << info.portName() //Se guarda todo en la lista
             << (!descripcion.isEmpty() ? descripcion : blankString)
             << (!fabricante.isEmpty() ? fabricante : blankString)
             << (!serial.isEmpty() ? serial : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

        ui->serialPortInfoListBox->addItem(list.first(), list); //Toda la lista de cosas queda guardada
    }
}

void preferencias::actualizarPreferencia(){
    actualPreferencia.nombre = ui->serialPortInfoListBox->currentText();

    if (ui->baudRateBox->currentIndex() == 4) {
        actualPreferencia.baudRate = ui->baudRateBox->currentText().toInt();
    } else {
        actualPreferencia.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    actualPreferencia.stringBaudRate = QString::number(actualPreferencia.baudRate);

    actualPreferencia.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    actualPreferencia.stringDataBits = ui->dataBitsBox->currentText();

    actualPreferencia.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    actualPreferencia.stringParity = ui->parityBox->currentText();

    actualPreferencia.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    actualPreferencia.stringStopBits = ui->stopBitsBox->currentText();

    actualPreferencia.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    actualPreferencia.stringFlowControl = ui->flowControlBox->currentText();

    actualPreferencia.robotBehave = (ui->behaviorList->itemData(ui->behaviorList->currentIndex()).toString());
}

