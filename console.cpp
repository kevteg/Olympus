#include "console.h"

Console::Console(QColor color_base, QColor color_text, QString nueva_linea, bool localEchoEnabled, QWidget *parent): QPlainTextEdit(parent), localEchoEnabled(true){
    this->nueva_linea = QByteArray(nueva_linea.toLatin1());
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    this->localEchoEnabled = localEchoEnabled;
    p.setColor(QPalette::Base, color_base);
    p.setColor(QPalette::Text, color_text);
    setPalette(p);


    if(nueva_linea != ">>")
        this->setStyleSheet("margin-top: 3px; border: 0px \
                            solid black; border-radius: 3px;");
    else
        this->setStyleSheet("margin-top: 3px; border: 0px \
                            solid white; border-radius: 3px; ");
    putData(this->nueva_linea);

}

void Console::putData(const QByteArray &data){
    insertPlainText(QString(data));
    if(data[data.length()-1] == '\n')
        putData(nueva_linea);

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::setLocalEchoEnabled(bool set){
    localEchoEnabled = set;
}

void Console::keyPressEvent(QKeyEvent *e){
    switch (e->key()) {
        case Qt::Key_Backspace:
            break;
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
            getFlechas(e);
            break;
        default:
            if(localEchoEnabled){
                QPlainTextEdit::keyPressEvent(e);
                if(e->key() == Qt::Key_Return){
                    putData(nueva_linea);
                    executeCommand(QString::fromStdString(command.str()));
                    qDebug() << QString::fromStdString(command.str());
                    command.str(std::string());
                }else
                    command << (char)e->key();
            }
            //if(e->key() != Qt::Key_Return)
            //emit getData(e->text().toLocal8Bit());
    }
}
void Console::executeCommand(QString command){
    if(command.toLower().contains("clc") || command.toLower().contains("clear"))
        limpiar();
    else if(command.toLower().contains("acerca") || command.toLower().contains("about"))
        emit showAbout();
    else if(command.toLower().contains("crs"))
        emit changeMstate();
    else if(command.toLower().contains("exit") || command.toLower().contains("salir"))
        emit exitProgram();
    else
        putData("Error[0]: Comando desconocido\n");
}

void Console::limpiar(){
    clear();
    putData(nueva_linea);
}

void Console::mousePressEvent(QMouseEvent *e){
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
