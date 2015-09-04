#-------------------------------------------------
#
# Project created by QtCreator 2015-03-29T12:53:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Olympus
TEMPLATE = app


greaterThan(QT_MAJOR_VERSION, 4) {
   QT       += widgets serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

SOURCES += main.cpp      \
        olymain.cpp      \
        robot.cpp        \
        preferencias.cpp \
        console.cpp      \
        messenger.cpp \
    verifytime.cpp \
    swarm.cpp \
    control_manual.cpp \
    qjoystick.cpp \
    qjoystickenumerator.cpp

HEADERS  += olymain.h      \
            robot.h        \
            preferencias.h \
            console.h      \
            protocolo.h    \
            messenger.h \
    verifytime.h \
    swarm.h \
    control_manual.h \
    qjoystick.h \
    qjoystickenumerator.h

FORMS    += olymain.ui  \
            preferencias.ui \ 
    qjoystickenumerator.ui

RESOURCES += \
    imagenes.qrc