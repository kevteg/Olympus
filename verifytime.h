#ifndef VERIFYTIME_H
#define VERIFYTIME_H

#include <QtCore>
#include "messenger.h"
#include "swarm.h"

//Esta clase se usa para verificar los envios
class verifyTime : public QObject{
    Q_OBJECT
public:
    explicit verifyTime(Console *terminal, swarm *swarm_object, messenger *sender, QQueue<QString>* messages_queue, QObject *parent = 0);
    ~verifyTime();
    void setSendEnable(bool);
signals:

public slots:
     void onTimeout();
private:
    swarm *swarm_object;
    messenger *sender;
    QQueue<QString>* messages_queue;
    Console *terminal;
};

#endif // VERIFYTIME_H
