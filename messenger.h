#ifndef MESSENGER_H
#define MESSENGER_H
#include <QSerialPort>
#include "preferencias.h"
//NOTA: DE RECIBIR MENSAJES DEBE ENCARGARSE ES OLYMAIN
class messenger
{
public:
    messenger(QSerialPort *serial);
    ~messenger();
    QSerialPort *getSerial();
    bool createConnection(preferencias::preferencia opt);
    void breakConnection();
    void sendMessage(const QByteArray &data);

private:
    QSerialPort *serial;

};

#endif // MESSENGER_H
