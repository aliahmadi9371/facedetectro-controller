#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "globals.h"
//----------------------------------------------------------------------------------------------------------------------

static int handler2 = -1;
//----------------------------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "";
    qDebug() << "*******************************************************";
    qDebug() << "*******************************************************";
    qDebug() << "****************** START APPLICATION ******************";
    qDebug() << "*******************************************************";
    qDebug() << "*******************************************************";
    qDebug() << "";

    qDebug() << "machineUniqueId = " << QSysInfo::machineUniqueId();
    QString machineUniqueId = QString::fromUtf8(QSysInfo::machineUniqueId());

    if(machineUniqueId == "7c0f6c39-52f9-409a-9af1-a1a7dc8fab5d"){


    //    GoToSysTray();
        InitValues();
        configApp();
        CreateConnections();
        RunRealTimeServer();
    }
}
//----------------------------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    qDebug() << "close real time server " << m.CloseNetwork(RealHndler);
    delete ui;
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::GoToSysTray()
{
    qDebug() << "**************************************************";
    qDebug() << "Show App on Sys Tray";
    icon.addFile("app.ico");

    SysTray.setIcon(icon);
    SysTray.setVisible(true);
    SysTray.setToolTip("Face RealTime Server");
    SysTray.show();
    qDebug() << "**************************************************";
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::InitValues()
{
    first = false;

    ConnectDelay = 0;
    ResponseDelay = 0;
    RelayDelay = 0;
    RealTimeDelay = 0;

    AccessPort = 0;
    AccessIp = ""; AccessIp.clear();

    DeviceServerPort = 0;

    FirstClose = false;

    LogRec = false;
    LogDel = false;

    RealHndler = -1;

    RequestThreshold = 0;
    Requests = 0;

    DeviceIdsList.clear();
    DeviceFirstCloseList.clear();
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::configApp()
{
    QSettings setting("./config.dll",QSettings::IniFormat);

    ConnectDelay = setting.value("App_Config/server_connect_delay_ms",100).toInt();
    ResponseDelay = setting.value("App_Config/server_response_delay_ms",100).toInt();
    RelayDelay = setting.value("App_Config/relay_on_delay_ms",100).toInt();
    RealTimeDelay = setting.value("App_Config/realtime_delay_s",5).toInt();
    AccessPort = quint16(setting.value("Access_Server/port",8080).toInt());
    AccessIp = setting.value("Access_Server/ip","127.0.0.1").toString();
    LogRec = setting.value("App_Config/log_offline_records",false).toBool();
    LogDel = setting.value("App_Config/log_offline_delete",false).toBool();
    DeviceServerPort = setting.value("Device_Server/port",81).toInt();
    FirstClose = setting.value("App_Config/close_door_first",false).toBool();
    RequestThreshold = setting.value("App_Config/threshold",false).toInt();

    if(!first){
        first = true;
        qDebug() << "";
        qDebug() << "####################################";
        qDebug() << "connection delay to tcp server = " << ConnectDelay;
        qDebug() << "response delay from tcp server = " << ResponseDelay;
        qDebug() << "realy on off delay = " << RelayDelay;
        qDebug() << "delay to empty offline logs = " << RealTimeDelay;
        qDebug() << "tcp server ip = " << AccessIp;
        qDebug() << "tcp server port = " << AccessPort;
        qDebug() << "LogRec = " << LogRec;
        qDebug() << "LogDel = " << LogDel;
        qDebug() << "FirstClose = " << FirstClose;
        qDebug() << "RequestThreshold = " << RequestThreshold;
        qDebug() << "####################################";
        qDebug() << "";
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::CreateConnections()
{
    //quit app
    connect(this, SIGNAL(Quit()), qApp, SLOT(quit()),Qt::QueuedConnection);

    connect(&m,SIGNAL(OnReceiveGLogData(QString,int,int,int,int,int,QDateTime,QString)),
            this,SLOT(Slot_OnReceiveGLogData(QString,int,int,int,int,int,QDateTime,QString)),
            Qt::QueuedConnection);

//    connect(&m,SIGNAL(OnReceiveGLogDataExtend(QString,QString,int,int,int,int,int,QDateTime,QString)),
//            this,SLOT(Slot_OnReceiveGLogDataExtend(QString,QString,int,int,int,int,int,QDateTime,QString)),
//            Qt::QueuedConnection);

//    connect(&m,SIGNAL(OnReceiveGLogText(QString, int, QString)),
//            this,SLOT(Slot_OnReceiveGLogText(QString, int, QString)),
//            Qt::QueuedConnection);

//    connect(&m,SIGNAL(OnReceiveGLogTextOnDoorOpen(QString, int, QString , QString)),
//            this,SLOT(Slot_OnReceiveGLogTextOnDoorOpen(QString, int, QString , QString)),
//            Qt::QueuedConnection);

//    connect(&m,SIGNAL(OnReceiveGLogTextAndImage(QString,int,QString,QString)),
//            this,SLOT(Slot_OnReceiveGLogTextAndImage(QString,int,QString,QString)),
//            Qt::QueuedConnection);
}
//----------------------------------------------------------------------------------------------------------------------

bool MainWindow::ResolveFunctions()
{
    bool ret = true;
    if (QLibrary::isLibrary("./FKAttend.dll")) { // FKAttend.dll
        lib.setFileName("./FKAttend.dll");
        lib.load();
        if (!lib.isLoaded()) {
            qDebug() << lib.errorString();
            ret = false;
        }
        else{
            qDebug() << "success load lib in thread id " << QThread::currentThreadId();
            FK_ConnectNet = reinterpret_cast<ConnectNet>(lib.resolve("FK_ConnectNet"));
            if (FK_ConnectNet){
//                qDebug() << "FK_ConnectNet Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_ConnectNet Can Not resolve : " << lib.errorString();
            }

            FK_DisConnect = reinterpret_cast<DisConnect>(lib.resolve("FK_DisConnect"));
            if (FK_DisConnect){
//                qDebug() << "FK_DisConnect Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_DisConnect Can Not resolve : " << lib.errorString();
            }

            FK_EmptyGeneralLogData = reinterpret_cast<EmptyGeneralLogData>(lib.resolve("FK_EmptyGeneralLogData"));
            if (FK_EmptyGeneralLogData){
//                qDebug() << "FK_EmptyGeneralLogData Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_EmptyGeneralLogData Can Not resolve : " << lib.errorString();
            }

            FK_SetDoorStatus = reinterpret_cast<SetDoorStatus>(lib.resolve("FK_SetDoorStatus"));
            if (FK_SetDoorStatus){
//                qDebug() << "FK_SetDoorStatus Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_SetDoorStatus Can Not resolve : " << lib.errorString();
            }

            FK_EnableDevice = reinterpret_cast<EnableDevice>(lib.resolve("FK_EnableDevice"));
            if (FK_EnableDevice){
//                qDebug() << "FK_EnableDevice Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_EnableDevice Can Not resolve : " << lib.errorString();
            }

            FK_LoadGeneralLogData = reinterpret_cast<LoadData>(lib.resolve("FK_LoadGeneralLogData"));
            if (FK_LoadGeneralLogData){
//                qDebug() << "FK_LoadGeneralLogData Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_LoadGeneralLogData Can Not resolve : " << lib.errorString();
            }

            FK_GetGeneralLogData_1 = reinterpret_cast<GetData>(lib.resolve("FK_GetGeneralLogData_1"));
            if (FK_GetGeneralLogData_1){
//                qDebug() << "FK_GetGeneralLogData_1 Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_GetGeneralLogData_1 Can Not resolve : " << lib.errorString();
            }
        }
    }
    else{
        ret = false;
    }
    return ret;
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::RunRealTimeServer()
{
    if(ResolveFunctions()){
        qDebug() << "app can resolve functions";

        qDebug() << "open real time network";
        RealHndler = m.OpenNetwork(DeviceServerPort);
        qDebug() << RealHndler;

        if(RealHndler > 0){
            qDebug() << "app can open real time network";
        }
        else{
            qDebug() << "app can not open real time network";
             emit this->Quit();
        }
    }
    else{
        qDebug() << "app can not resolve functions";
         emit this->Quit();
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::Slot_OnReceiveGLogData(QString _deviceip, int _deviceport, int _deviceid, int _userid, int _verifymode, int _iomode, QDateTime _datetime, QString _serialnumber)
{
    qDebug() << "";
    long long difTime = QDateTime::currentDateTime().toSecsSinceEpoch() - _datetime.toSecsSinceEpoch();
    qDebug() << "Slot_OnReceiveGLogData " << _deviceip << ":" << _deviceport << "_" << _deviceid << "  " << _userid;
    qDebug() << "difTime = " << difTime;
    if( difTime > RealTimeDelay  ){
        if(LogDel || LogRec || FirstClose){
            int handler = ConnectToDevice(_deviceid,_deviceip.toLocal8Bit().data(),_deviceport,5000,0,0,1262);
            if(LogRec)
                SaveLogToFile(handler,1);
            if(LogDel)
                ClearDeviceLog(handler);
//            if(FirstClose)
//                CheckAndCloseDoor(handler,_deviceid);
            DisconnectFromDevice(handler);
        }
    }
    else if(_userid != 0 && _userid != 12345){
        qDebug() << "device ip " << _deviceip << " device port " << _deviceport << " device id " << _deviceid << " user id " << _userid ;
//        qDebug() << "at " << _datetime;
        qDebug() << "diif time = " << difTime;

        Requests++;
        qDebug() << "thresoulds = " << Requests;
        if(Requests >= RequestThreshold){
            qDebug() << "#################thresoulds#################";
            emit this->Quit();
        }
        else{
            DeviceThread = new QThread();
            myDeviceWorker = new DeviceWorker();

            connect(myDeviceWorker, SIGNAL(Sig_FinishWorker(DeviceWorker*, QThread*)), this, SLOT(Slot_DeleteThreadAndWorker(DeviceWorker*, QThread*)),Qt::QueuedConnection);
            connect(myDeviceWorker, SIGNAL(Sig_Quit()), qApp, SLOT(quit()),Qt::QueuedConnection);
            DeviceThread->start();
            myDeviceWorker->moveToThread(DeviceThread);
            configApp();
            myDeviceWorker->Sig_StartWorker(DeviceThread,_userid,_deviceid,AccessIp,AccessPort,ConnectDelay,ResponseDelay,RelayDelay,_deviceip,_deviceport);
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------

//void MainWindow::Slot_OnReceiveGLogDataExtend(QString _rootip, QString _deviceip, int _deviceport, int _deviceid, int _userid, int _verifymode, int _iomode, QDateTime _datetime, QString _serialnumber)
//{
////    qDebug() << "";
////    qDebug() << "Slot_OnReceiveGLogDataExtend";
////    qDebug() << "root ip " << _rootip;
////    qDebug() << "device ip " << _deviceip;
//}
//----------------------------------------------------------------------------------------------------------------------

//void MainWindow::Slot_OnReceiveGLogTextAndImage(QString _ip, int _port, QString _log, QString _image)
//{
////    qDebug() << "";
////    qDebug() << "Slot_OnReceiveGLogTextAndImage";
//}
//----------------------------------------------------------------------------------------------------------------------

//void MainWindow::Slot_OnReceiveGLogText(QString _deviceip, int _deviceport, QString _message)
//{
//    qDebug() << "";
//    qDebug() << "Slot_OnReceiveGLogText";
//}
////----------------------------------------------------------------------------------------------------------------------

//void MainWindow::Slot_OnReceiveGLogTextOnDoorOpen(QString _deviceip, int _deviceport, QString _message, QString _image)
//{
////    qDebug() << "";
////    qDebug() << "Slot_OnReceiveGLogTextOnDoorOpen";
//}
//----------------------------------------------------------------------------------------------------------------------

bool MainWindow::LoadingData(int _devicehandler, int _flagNew)
{
    int ret = FK_LoadGeneralLogData(_devicehandler, _flagNew);
    if(ret != 1)
        return false;
    else
        return true;
}
//----------------------------------------------------------------------------------------------------------------------

QString MainWindow::GetingData(int _devicehandler)
{
    int user_id;
    int verify_mode;
    int io_mode;
    int dyear;
    int dmonth;
    int dday;
    int thour;
    int tminute;
    int tsec;
    int ret = FK_GetGeneralLogData_1(_devicehandler, &user_id, &verify_mode, &io_mode, &dyear, &dmonth, &dday, &thour, &tminute, &tsec);
    if(ret != 1)
        return "";
    else{
        QString str = "user id: " + QString::number(user_id) + " verify mode:" + QString::number(verify_mode) + " io mode:" + QString::number(io_mode) + " date _ time:" +  QString::number(dyear) + "/" + QString::number(dmonth) + "/" + QString::number(dday) + "_" + QString::number(thour) + ":" + QString::number(tminute) + ":" + QString::number(tsec);
        qDebug() << str;
        return str;
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::SaveLogToFile(int _devicehandler, int _flagNew)
{
    if(LoadingData(_devicehandler,_flagNew)){
        QString res = "";
        QString date = QDate::currentDate().toString("yyyy-MM-dd");
        QString time = QTime::currentTime().toString("hh-mm-ss");
        bool write = false;;
        do {
            res = GetingData(_devicehandler);
            if(res != ""){
                if(!QDir("./Offline_Recors").exists()){
                    QDir dir;
//                    qDebug() << "Create Offline_Recors ****************************" << dir.mkdir("./Offline_Recors");
                }
                if(!QDir(date).exists()){
                    QDir dir;
//                     qDebug() << "Create Date ****************************" << dir.mkdir("./Offline_Recors/"+date);
                }
                file.setFileName("./Offline_Recors/"+date+"/"+time);
                if (file.isOpen())
                {
                    file.write(res.toLocal8Bit().data(),res.length());
                    file.write("\r\n",2);
                    file.close();
                }
                else {
                    if(file.open(QIODevice::ReadWrite | QIODevice::Append))
                        write = true;
                    file.write(res.toLocal8Bit().data(),res.length());
                    file.write("\r\n",2);
                    file.close();

                }
            }
        } while (res != "");
        if(write)
            file.close();
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::Slot_DeleteThreadAndWorker(DeviceWorker* _workerpointer, QThread* _parentthread)
{
    qDebug() << "deleting thread";
    _parentthread->exit(0);
    _parentthread->terminate();
    _parentthread->deleteLater();
    delete _workerpointer;
    delete _parentthread;
    qDebug() << "delete thread";
}
//----------------------------------------------------------------------------------------------------------------------

int MainWindow::ConnectToDevice(int _deviceid, QString _deviceip, int _deviceport, int _devicetimeout, int _deviceprotocol, int _devicepass, int _devicelicense)
{
    int handler = FK_ConnectNet(_deviceid,_deviceip.toLocal8Bit().data(),_deviceport,_devicetimeout,_deviceprotocol,_devicepass,_devicelicense);
    return handler;
}
//----------------------------------------------------------------------------------------------------------------------

bool MainWindow::DisconnectFromDevice(int _devicehandler)
{

    if(_devicehandler > 0){
        int ret = FK_DisConnect(_devicehandler);
        if(ret == 1)
            return true;
        else
            return false;
    }
    else
        return false;

}
//----------------------------------------------------------------------------------------------------------------------

bool MainWindow::OpenDeviceDoor(int _devicehandler)
{
    if(_devicehandler > 0){
        int ret = FK_SetDoorStatus(_devicehandler,1);
        if(ret == 1)
            return true;
        else
            return false;
    }
    else
        return false;

}
//----------------------------------------------------------------------------------------------------------------------

bool MainWindow::CloseDeviceDoor(int _devicehandler)
{
    if(_devicehandler > 0){
        int ret = FK_SetDoorStatus(_devicehandler,2);
        if(ret == 1)
            return true;
        else
            return false;
    }
    else
        return false;
}
//----------------------------------------------------------------------------------------------------------------------

bool MainWindow::ClearDeviceLog(int _devicehandler)
{
    if(_devicehandler > 0){
        int ret = FK_EmptyGeneralLogData(_devicehandler);
        if(ret == 1)
            return true;
        else
            return false;
    }
    else
        return false;
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::CheckAndCloseDoor(int _devicehandler, int _deviceid)
{
    int item_index = FindDeviceIdFromList(_deviceid);
    if(item_index == 0){
        AppendItemToDeviceIdsList(_deviceid);
        int item_index_2 = DeviceIdsList.count() - 1;
        CloseDeviceDoor(_devicehandler);
        DeviceFirstCloseList.replace(item_index_2,true);
    }
    else{
        if( !DeviceFirstCloseList.at(item_index) ){
            CloseDeviceDoor(_devicehandler);
            DeviceFirstCloseList.replace(item_index,true);
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::AppendItemToDeviceIdsList(int _deviceid)
{
    int item_index = FindDeviceIdFromList(_deviceid);
    if(item_index != 0){
        qDebug() << "device id " << _deviceid << " added before";
    }
    else{
        qDebug() << "device id " << _deviceid << " not added before and is adding now";
        DeviceIdsList.append(_deviceid);
        DeviceFirstCloseList.append(false);
    }
}
//----------------------------------------------------------------------------------------------------------------------

void MainWindow::DeleteItemFromDeviceIdsList(int _deviceid)
{
    int item_index = FindDeviceIdFromList(_deviceid);
    if(item_index != 0){
        qDebug() << "device id " << _deviceid << " added before and is deleting now";
        DeviceIdsList.removeAt(item_index);
        DeviceFirstCloseList.removeAt(item_index);
    }
    else{
        qDebug() << "device id " << _deviceid << " not added before";
    }
}
//----------------------------------------------------------------------------------------------------------------------

int MainWindow::FindDeviceIdFromList(int _deviceid)
{
    int item_index = 0;
    for(int i=0; i<DeviceIdsList.count(); i++){
        if(DeviceIdsList.at(i) == _deviceid){
            item_index = i;
            break;
        }
    }
    return item_index;
}
//----------------------------------------------------------------------------------------------------------------------

bool MainWindow::ReturnFirstCloseOfDevice(int _deviceid)
{
    bool ret = false;
    int item_index = FindDeviceIdFromList(_deviceid);
    if(item_index == 0)
        return ret;
    else{
        return DeviceFirstCloseList.at(item_index);
    }
}
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//test functions
void MainWindow::on_pushButton_3_clicked()
{
    handler2 = FK_ConnectNet(1,"192.168.1.45",5005,5000,0,0,1262);
}

void MainWindow::on_pushButton_4_clicked()
{
    FK_DisConnect(handler2);
}

void MainWindow::on_pushButton_clicked()
{
    FK_SetDoorStatus(handler2,1);
}

void MainWindow::on_pushButton_2_clicked()
{
    FK_SetDoorStatus(handler2,2);
}

void MainWindow::one()
{
    FK_SetDoorStatus(handler2,1);
}

void MainWindow::two()
{
    FK_SetDoorStatus(handler2,2);
}

void MainWindow::three()
{
    handler2 = FK_ConnectNet(1,"192.168.1.45",5005,5000,0,0,1262);
}

void MainWindow::four()
{
    FK_DisConnect(handler2);
}
//test functions
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
