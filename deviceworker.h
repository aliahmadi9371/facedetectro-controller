#ifndef DEVICEWORKER_H
#define DEVICEWORKER_H

#include <QObject>
#include <QLibrary>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include "globals.h"

class DeviceWorker : public QObject
{
    Q_OBJECT
public:
    explicit DeviceWorker(QObject *parent = nullptr);



    bool ResolveFunctions();

    int ( *FK_ConnectNet ) (int, char*, int, int, int, int, int);
    int ( *FK_DisConnect ) (int);
    int ( *FK_EmptyGeneralLogData ) (int);
    int ( *FK_SetDoorStatus ) (int, int);
    int ( *FK_EnableDevice) (int , BYTE);


    int ConnectToDevice(int _deviceid, QString _deviceip, int _deviceport, int _devicetimeout, int _deviceprotocol, int _devicepass, int _devicelicense);
    bool DisconnectFromDevice(int _devicehandler);
    bool OpenDeviceDoor(int _devicehandler);
    bool CloseDeviceDoor(int _devicehandler);
    bool ClearDeviceLog(int _devicehandler);

    QTcpSocket* myTcpSocket;

signals:
    void Sig_StartWorker(QThread* _parentthread,int _userid, int _deviceid, QString _hostip, quint16 _hostport, int _delayconnect, int _delayread, unsigned long _delayrelay, QString _ip, long _port);
    void Sig_FinishWorker(DeviceWorker* _workerpointer, QThread* _parentthread);
    void Sig_Quit();
public slots:
    void Slot_StartWorker(QThread* _parentthread, int _userid, int _deviceid, QString _hostip, quint16 _hostport, int _delayconnect, int _delayread, unsigned long _delayrelay, QString _ip, long _port);

public slots:
};

#endif // DEVICEWORKER_H
