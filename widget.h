#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLibrary>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTcpSocket>
#include <QTcpServer>
#include <QProcess>
#include "qvcamserver.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void NewConnect();
    void ReadMessage();
    void SendMsg();
    void TcpClose();


    int flir_init();
    int flir_connect(int auto);
    int flir_close();
    int flir_getfrequency();
    int flir_setfrequency(int frequency);
    int flir_setfilter(int index);
    int flir_getit(int index);
    int flir_setit(int index, int it);
    int flir_setnuc();

private:
    Ui::Widget *ui;
    QLibrary *mylib;      //加载动态库
    QTcpServer *server;
    QTcpSocket *tcp;

    QVCamServer vcam;
    bool init_flag;
    int ITchannels;

};

#endif // WIDGET_H
