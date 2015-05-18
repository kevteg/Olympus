/**
 * @file console.h
 * @brief Librería de las consolas de los robots
 * @author Kevin Hernández, Ángel Gil
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QtCore/QDebug>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
class Console : public QPlainTextEdit
{
    Q_OBJECT
signals:
    /**
     * @brief Señal usada para subir el mensaje de la terminal al programa principal
     * @param const QByteArray &data: Se envia lo que se escribe directamente
     */
    void getData(const QByteArray &data);
    /**
     * @brief Señal usada para indicar que se ha presionado alguna tecla
     * @param e: tecla presionada
     */
    void getFlechas(QKeyEvent *e);
    /**
     * @brief Señal usada para mostrar about en olymain
     */
    void showAbout();
    /**
     * @brief Señal usada para mostrar o no los mensajes que llegan al programa
     */
    void changeMstate();
    /**
     * @brief Señal usada para salir del programa
     */
    void exitProgram();

public:
    /**
     * @brief Constructor de la consola
     * @param QWidget *parent = 0: Parametro de qt, padre de la consola
     */
    explicit Console(QColor color_base, QColor color_text, QString nueva_linea, bool localEchoEnabled, QWidget *parent = 0);
    /**
     * @brief Poner datos sobre la consola
     * @param const QByteArray &data: String que se pondrá en la consola
     */
    void putData(const QByteArray &data);
    /**
     * @brief Ver lo que se escribe
     * @param bool set: si es true se podrá ver lo que se ha escrito al robot
     */
    void setLocalEchoEnabled(bool set);
    void executeCommand(QString command);
private slots:
    /**
     * @brief Limpiar el texto de la terminal
     */
    void limpiar();
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);

private:
    stringstream command;
    bool localEchoEnabled;
    QByteArray nueva_linea;

};

#endif // CONSOLE_H
