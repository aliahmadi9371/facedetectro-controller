#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "realsvrocxtcplib.h"
#include <QList>
#include <QLibrary>
#include <QTcpSocket>
#include <QThread>
#include <QSettings>
#include "deviceworker.h"
#include <ocidl.h>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QSysInfo>

using namespace RealSvrOcxTcpLib;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool first;

    QThread* DeviceThread;
    DeviceWorker* myDeviceWorker;

    void GoToSysTray();
    void InitValues();
    void configApp();
    void CreateConnections();
    bool ResolveFunctions();
    void RunRealTimeServer();


    int ConnectDelay;
    int ResponseDelay;
    int RelayDelay;
    int RealTimeDelay;

    quint16 AccessPort;
    QString AccessIp;

    int DeviceServerPort;

    bool FirstClose;

    bool LogRec;
    bool LogDel;

    RealSvrOcxTcp m;
    int RealHndler;

    QList<int> DeviceIdsList;
    QList<bool> DeviceFirstCloseList;

    QIcon icon;
    QSystemTrayIcon SysTray;

    int ( *FK_ConnectNet ) (int, char*, int, int, int, int, int);
    int ( *FK_DisConnect ) (int);
    int ( *FK_EmptyGeneralLogData ) (int);
    int ( *FK_SetDoorStatus ) (int, int);
    int ( *FK_EnableDevice) (int , BYTE);
    int ( *FK_LoadGeneralLogData)(int ,int);
    int ( *FK_GetGeneralLogData_1)(int, int*, int*, int*, int*, int*, int*, int*, int*, int*);


    QFile file;
    QTcpSocket myTcpSocket;
    QLibrary lib;

    int Requests;
    int RequestThreshold;


    int ConnectToDevice(int _deviceid, QString _deviceip, int _deviceport, int _devicetimeout, int _deviceprotocol, int _devicepass, int _devicelicense);
    bool DisconnectFromDevice(int _devicehandler);
    bool OpenDeviceDoor(int _devicehandler);
    bool CloseDeviceDoor(int _devicehandler);
    bool ClearDeviceLog(int _devicehandler);
    bool LoadingData(int _devicehandler, int _flagNew);
    QString GetingData(int _devicehandler);
    void SaveLogToFile(int _devicehandler, int _flagNew);
    void CheckAndCloseDoor(int _devicehandler, int _deviceid);

    void one();
    void two();
    void three();
    void four();

    void AppendItemToDeviceIdsList(int _deviceid);
    void DeleteItemFromDeviceIdsList(int _deviceid);
    int FindDeviceIdFromList(int _deviceid);
    bool ReturnFirstCloseOfDevice(int _deviceid);




public slots:
    void Slot_OnReceiveGLogData(QString _deviceip, int _deviceport, int _deviceid, int _userid, int _verifymode, int _iomode, QDateTime _datetime, QString _serialnumber);
//    void Slot_OnReceiveGLogDataExtend(QString _rootip, QString _deviceip, int _deviceport, int _deviceid, int _userid, int _verifymode, int _iomode, QDateTime _datetime, QString _serialnumber);
//    void Slot_OnReceiveGLogTextAndImage(QString _ip, int _port, QString _log, QString _image);
//    void Slot_OnReceiveGLogText(QString _deviceip, int _deviceport, QString _message);
//    void Slot_OnReceiveGLogTextOnDoorOpen(QString _deviceip, int _deviceport, QString _message, QString _image);

    void Slot_DeleteThreadAndWorker(DeviceWorker* _workerpointer, QThread* _parentthread);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
signals:
    void Quit();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
