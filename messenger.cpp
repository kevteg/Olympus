#include "messenger.h"

messenger::messenger(QSerialPort *serial){
    this->serial = serial;
}
QSerialPort* messenger::getSerial(){
    return serial;
}

bool messenger::createConnection(preferencias::preferencia opt){
    serial->setPortName(opt.nombre);
    serial->setBaudRate(opt.baudRate);
    serial->setDataBits(opt.dataBits);
    serial->setParity(opt.parity);
    serial->setStopBits(opt.stopBits);
    serial->setFlowControl(opt.flowControl);
    return serial->open(QIODevice::ReadWrite);
}

void messenger::breakConnection(){
    serial->close();
}

void messenger::sendMessage(const QByteArray &data){
     serial->write(data);
}

messenger::~messenger(){
    delete serial;
}

