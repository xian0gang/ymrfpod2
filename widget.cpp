/**************************************
 * 热像仪2 控制
 *接受来自热像仪1控制软件的命令
 *ip:192.168.2.130
 *port:9000
 * ********************************/

#include "widget.h"
#include "ui_widget.h"

#include "FLIR_LIB.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    init_flag = false;

    server = new QTcpServer();
    tcp = new QTcpSocket();
    server->listen(QHostAddress::Any, 9000);
    ui->textBrowser->append("port:9000");
    connect(server,SIGNAL(newConnection()),this,SLOT(NewConnect()));

    ui->textBrowser->document()->setMaximumBlockCount(1500);

    vcam.init(0);

}

void Widget::NewConnect()
{
    ui->textBrowser->append("new connect");
    tcp = server->nextPendingConnection();

    connect(tcp, SIGNAL(readyRead()), this, SLOT(ReadMessage()));
    connect(tcp, SIGNAL(disconnected()), this, SLOT(TcpClose()));
}


void Widget::ReadMessage()
{
    QByteArray ba;
    ba = tcp->readAll();
    qDebug()<<"read:"<<ba;
    ui->textBrowser->append(ba);
    //热像仪1 模块初始化
    if(ba == "FLIR_init2")
    {
        flir_init();
        ui->textBrowser->append("模块初始化完成");
        QString str2 = "FLIR_init2";
        QByteArray data(str2.toLatin1());
        tcp->write(data);
    }
    if(init_flag)
    {
        //热像仪1 设置频率
        if(ba.indexOf("FLIR_setfrequency2:") >= 0)
        {
            QString str = ba;
            QString num = str.section(':', 1, 1);
            int success  = flir_setfrequency(num.toInt());
            QString str2 = "FLIR_setfrequency2:";
            str2 = str2 + QString::number(success, 10);
            QByteArray data(str2.toLatin1());
            tcp->write(data);
        }

        //热像仪1 获取频率
        if(ba.indexOf("FLIR_getfrequency2") >= 0)
        {
            int frequency = flir_getfrequency();
            QString str = "FLIR_getfrequency2:";
            str = str + QString::number(frequency, 10);
            QByteArray data(str.toLatin1());
            tcp->write(data);
        }

        //热像仪1 连接
        if(ba.indexOf("FLIR_connect2:") >= 0)
        {
            QString str = ba;
            QString num = str.section(':', 1, 1);
            flir_connect(num.toInt());
        }

        //滤片
        if(ba.indexOf("FLIR_filter2:") >= 0)
        {

            QString str = ba;
            QString num = str.section(':', 1, 1);
            qDebug()<<num.toInt();
            int success = flir_setfilter(num.toInt());

            QString str2 = "FLIR_filter2:";
            str2 = str2 + QString::number(success, 10);
            QByteArray data(str2.toLatin1());
            tcp->write(data);
        }

        if(ba.indexOf("FLIR_close2") >= 0)
        {
            qDebug()<<"FLIR_close2";
            flir_close();
        }

        if(ba.indexOf("FLIR_getit2:") >= 0)
        {
            QString str = ba;
            QString num = str.section(':', 1, 1);
            qDebug()<<num.toInt();

            int it = flir_getit(num.toInt());

//            QString str = "FLIR_getit2:";
            str = str + ":" + QString::number(it, 10);
            QByteArray data(str.toLatin1());
            tcp->write(data);

            if(it == 0)
            {
                ui->textBrowser->append("获取积分时间为0或者失败");
            }
            else
            {
                ui->textBrowser->append("获取积分时间成功");
            }
        }

        if(ba.indexOf("FLIR_setit2:") >= 0)
        {
            QString str = ba;
            QString index = str.section(':', 1, 1);
            QString it = str.section(':', 2, 2);

            int success = flir_setit(index.toInt(), it.toInt());

            QString str2 = "FLIR_setit2:";
            str2 = str2 + index +":" + QString::number(success, 10);
            QByteArray data(str2.toLatin1());
            tcp->write(data);
        }

        if(ba.indexOf("FLIR_SetMultiTi2:") >= 0)
        {
            QString str = ba;
            QString num = str.section(':', 1, 1);
            QString index = str.section(':', 2, 2);
            LONG    nChannel = num.toLong(); //积分通道
            int success = vcam.SetMultiTi(nChannel);
            if(success == 0)
            {
                ui->textBrowser->append("开启积分通道失败");
            }
            else
            {
                ui->textBrowser->append("开启积分通道成功");
            }
            QString str2 = "FLIR_SetMultiTi2:";
            str2 = str2 + index + ":" + QString::number(success, 10);
            QByteArray data(str2.toLatin1());
            tcp->write(data);
        }

        if(ba.indexOf("FLIR_setnuc2") >= 0)
        {
            int success = flir_setnuc();
            QString str2 = "FLIR_setnuc2:";
            str2 = str2 + QString::number(success, 10);
            QByteArray data(str2.toLatin1());
            tcp->write(data);
        }
    }
    else
    {
        QString str2 = "FLIR_warning2:init";
        QByteArray data(str2.toLatin1());
        tcp->write(data);
    }
}

void Widget::TcpClose()
{
    tcp->deleteLater();
    qDebug() <<"TcpClose";
    ui->textBrowser->append("Tcp Close");
}

void Widget::SendMsg()
{
    QString str;
//    str += ui->textEdit->toPlainText();
//    tcp->write(str.toLatin1());
}


Widget::~Widget()
{
//    qApp->closeAllWindows();
//    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
//    qDebug()<<"exit";

    flir_close();
    delete ui;
}


//初始化
int Widget::flir_init()
{
//    FLIR_LIBInitialize();

//    mylib = new QLibrary("FLIR_LIB");
//    mylib->load();
//    if(!mylib->isLoaded())
//    {
//        qDebug()<<"load FLIR_Matlab_SDK_Lib failed";
//    }
//    typedef void (*fun)();
//    fun FLIR_Init = (fun)mylib->resolve("?FLIR_Init@@YAXXZ");
//    FLIR_Init();

//    init_flag = true;
}

int Widget::flir_connect(int num)
{
//    typedef void (*fun1)(int nargout, mwArray& bSuccess, const mwArray& bAutomaticConnection);
//    fun1 FLIR_Camera_Connect = (fun1)mylib->resolve("?FLIR_Camera_Connect@@YAXHAAVmwArray@@ABV1@@Z");
//    int connect = 1;

//    mwArray mwa(1,1,mxINT32_CLASS);
//    mwArray mwb(1,1,mxINT32_CLASS);
//    //bAutomaticConnection 1 自动连接
//    //bAutomaticConnection 0 手动连接
//    int a[1] = {num};

//    mwb.SetData(a,1);
//    FLIR_Camera_Connect(connect, mwa, mwb);
//    int success = mwa.Get(1,1);
////    qDebug()<<success;
//    int success2 = mwb.Get(1,1);
////    qDebug()<<success2;

//    int aa = 0;
//    ITchannels = vcam.GetMultiNbrChannel((int&)aa);
//    if(ITchannels != 4)
//    {
//        LONG    nChannel =0x04; //积分通道
//        int success3 = vcam.SetMultiTi(nChannel);
//        if(success3 == 0)
//        {
//            ui->textBrowser->append("开启多积分通道失败");
//        }
//        else
//        {
//            ui->textBrowser->append("开启多积分通道成功");
//        }
//    }

//    QString str3 = "FLIR_GetMultiNbrChannel2:";
//    str3 = str3 + QString::number(ITchannels, 10);
//    QByteArray data3(str3.toLatin1());
//    tcp->write(data3);
//    Sleep(1000);

//    return success;
}


int Widget::flir_close()
{
//    typedef void (*fun1)();
//    fun1 FLIR_Close = (fun1)mylib->resolve("?FLIR_Close@@YAXXZ");
//    FLIR_Close();
//    init_flag = false;
}



int Widget::flir_getfrequency()
{
//    typedef void (*fun1)(int nargout, mwArray& bSuccess, const mwArray& bAutomaticConnection);
//    fun1 FLIR_Camera_GetFrequency = (fun1)mylib->resolve("?FLIR_Camera_GetFrequency@@YAXHAAVmwArray@@0@Z");
//    int connect = 2;

//    mwArray mwa(1,1,mxINT32_CLASS);
//    mwArray mwb(1,1,mxINT32_CLASS);
//    FLIR_Camera_GetFrequency(connect, mwa, mwb);
//    int success = mwa.Get(1,1);
//    qDebug()<<success;
//    int success2 = mwb.Get(1,1);
//    qDebug()<<success2;
//    if(success == 0)
//    {
//        qDebug("get Frequency failed");
//        return 0;
//    }
//    return  success2;
}

int Widget::flir_setfrequency(int frequency)
{
//    typedef void (*fun1)(int nargout,
//                         mwArray& bSuccess,
//                         mwArray& bSucceededToSetFrequency,
//                         mwArray& fActualFrequency,
//                         const mwArray& fFrequency,
//                         const mwArray& bAutoFrequency);
//    fun1 FLIR_Camera_SetFrequency = (fun1)mylib->resolve("?FLIR_Camera_SetFrequency@@YAXHAAVmwArray@@00ABV1@1@Z");
//    int connect = 2;

//    mwArray mwa(1,1,mxINT32_CLASS);
//    mwArray mwb(1,1,mxINT32_CLASS);
//    mwArray mwc(1,1,mxINT32_CLASS);
//    mwArray mwd(1,1,mxINT32_CLASS);
//    mwArray mwe(1,1,mxINT32_CLASS);
//    int a[1] = {0};
//    mwd.SetData(a,1);
//    int b[1] = {frequency};
//    mwc.SetData(b,1);

//    FLIR_Camera_SetFrequency(connect, mwa, mwb, mwe, mwc, mwd);
//    int success = mwa.Get(1,1);
//    int success2 = mwb.Get(1,1);

//    return success2;
}

int Widget::flir_setfilter(int index)
{
//    typedef void (*fun1)(int nargout,
//                         mwArray& bSuccess,
//                         const mwArray& dwFilterIndex);
//    fun1 FLIR_Camera_SetFilter = (fun1)mylib->resolve("?FLIR_Camera_SetFilter@@YAXHAAVmwArray@@ABV1@@Z");
//    int connect = 1;

//    mwArray mwa(1,1,mxINT32_CLASS);
//    mwArray mwb(1,1,mxINT32_CLASS);
//    int b[1] = {index};
//    mwb.SetData(b,1);

//    FLIR_Camera_SetFilter(connect, mwa, mwb);
//    int success = mwa.Get(1,1);

//    return success;
}

//获取积分时间
int Widget::flir_getit(int index)
{
//    typedef void (*fun1)(int nargout,
//                         mwArray& bSuccess,
//                         mwArray& fIT,
//                         const mwArray& nChannel);
//    fun1 FLIR_Camera_GetIT = (fun1)mylib->resolve("?FLIR_Camera_GetIT@@YAXHAAVmwArray@@0ABV1@@Z");
//    int connect = 1;

//    mwArray mwa(1,1,mxINT32_CLASS);
//    mwArray mwb(1,1,mxINT32_CLASS);
//    mwArray mwc(1,1,mxINT32_CLASS);
//    int c[1] = {index};
//    mwc.SetData(c,1);

//    FLIR_Camera_GetIT(connect, mwa, mwb, mwc);
//    int success = mwa.Get(1,1);
//    int it = mwb.Get(1,1);

//    if(success == 0)
//    {
//        ui->textBrowser->append("获取积分时间失败！");
//        return 0;
//    }

//    return it;
}

int Widget::flir_setit(int index, int it)
{    
//    ULONG dwIntegration = it;
//    ULONG dwDelay = 0x00;

//    ULONG bAutoFrequency = FALSE;
//    uint success2 = vcam.SetIntegration(dwIntegration, dwDelay, index, bAutoFrequency);
//    return success2;
}

int Widget::flir_setnuc()
{
//    uint hr;
//    uint ret;

//    //计算1point NUC
//    uint bNUCIsToDo = TRUE; // NUC is to be calculate
//    int nNucType = 1; // 1 pt NUC type
//    int nMethode = 2; // BB method
//    uint bKeepGain = FALSE; // Keep previous Gain option
//    int nNumberOfAverageFrame = 10; // Number of frame
//    uint bSave = FALSE; //Save after calcul option


//    ret=vcam.ScanCams();
//    ret=vcam.ConnectToCamScanned(0);
//    if (!ret)
//    {
//        Sleep(2000);
//        ret=vcam.ScanCams();
//        ret=vcam.ConnectToCamScanned(0);
//    }
//    Sleep(1000);

//    //Set NUC to be Caluclate
//    hr = vcam.SetDoNuc(bNUCIsToDo);
//    qDebug()<<hr;
//    //Choose 1 pt NUC type
//    hr = vcam.SetNucType(nNucType);
//    qDebug()<<hr;
//    //Choose Black body method
//    //Note that you can only do one point NUC with BB method or shutter one
//    hr = vcam.SetMethode(nMethode);
//    qDebug()<<hr;
//    //Choose Keep previous Gain
//    hr = vcam.SetKeepGain(bKeepGain);
//    qDebug()<<hr;
//    //Set Average number of frames
//    hr = vcam.SetAverageFrames(nNumberOfAverageFrame);
//    qDebug()<<hr;
//    //Don’t save after NUC calcul
//    hr = vcam.SetSaveAfterUpdate(bSave);
//    qDebug()<<hr;
//    //Calculate NUC and update informations
//    hr =vcam.UpdateNuc();
//    qDebug()<<hr;

//    if(hr == 0)
//    {
//        ui->textBrowser->append("非均匀性矫正失败！");
//    }
//    else
//    {
//        ui->textBrowser->append("非均匀性矫正成功！");
//    }
//    return hr;
}
