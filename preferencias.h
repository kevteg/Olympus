#ifndef PREFERENCIAS_H
#define PREFERENCIAS_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include "qjoystickenumerator.h"

QT_USE_NAMESPACE
QT_BEGIN_NAMESPACE

namespace Ui {
    class preferencias;
}

class QIntValidator;

QT_END_NAMESPACE

class preferencias : public QDialog{
    Q_OBJECT

public:
    struct preferencia{
        QString nombre;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        QString robotBehave;
        QJoystick *joystick;
        bool localEchoEnabled;
    };

    explicit preferencias(QWidget *parent = 0);
    ~preferencias();

    preferencia Preferencia() const;
signals:
    void info_update(preferencias::preferencia set);
private slots:
    /**
     * @brief Mostrar información del puerto seleccionado en la lista
     * @param idx: Indice del puerto en la lista
     */
    void infoPuerto(int idx);
    /**
     * @brief Guardar las nuevas preferencias
     */
    void aplicar();
    /**
     * @brief Verificar que se ingresan baudios por el teclado
     * @param idx: indice del espacio a modificar
     */
    void checkCustomBaudRatePolicy(int idx);
    /**
     * @brief Selección del Joystick
     */
    void show_joy();

private:
    /**
     * @brief Agregar a las listas los parámetros a seleccionar
     */
    void mostrar_parametrosPuertos();
    /**
     * @brief Por cada puerto se agrega la información a la lista para mostrarlo cuando se selecciona
     */
    void mostrar_informacionPuertos();
    /**
     * @brief Actualizar las preferencias seleccionadas
     */
    void actualizarPreferencia();

private:
    Ui::preferencias *ui;
    preferencia actualPreferencia;
    QIntValidator *intValidator;
};

#endif
