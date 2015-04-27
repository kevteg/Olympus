#include "olymain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    olymain w;
    w.show();

    return a.exec();
}
