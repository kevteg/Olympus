#include "olymain.h"
#include <QApplication>
#include <QtCore>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    olymain w;
    w.show();
    return a.exec();
}
