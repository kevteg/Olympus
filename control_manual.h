#ifndef CONTROL_MANUAL_H
#define CONTROL_MANUAL_H

#include <QMainWindow>
#include <QToolButton>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#define n_buttons_sensors 2

class control_manual : public QMainWindow
{
    Q_OBJECT
public:
    explicit control_manual(QString robot_name, QWidget *parent = 0);
    ~control_manual();

signals:

public slots:

private:
    QToolButton     **arrows_buttons;
    QToolButton     **sensors_buttons;
    QComboBox        *behaviors;


};

#endif // CONTROL_MANUAL_H
