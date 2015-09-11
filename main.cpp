#include "olymain.h"
#include <QApplication>
#include <QtCore>

bool protocolo::sender_safe  = true;
bool protocolo::queue_safe   = true;
bool protocolo::send_enabled = false;
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    olymain w;
    w.show();
    return a.exec();
}
