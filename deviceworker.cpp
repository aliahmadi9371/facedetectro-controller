#include "deviceworker.h"
#include <ocidl.h>


DeviceWorker::DeviceWorker(QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(Sig_StartWorker(QThread*,int, int, QString, quint16, int, int, unsigned long, QString, long)),
            this,SLOT(Slot_StartWorker(QThread*,int, int, QString, quint16, int, int, unsigned long, QString, long))
                      ,Qt::QueuedConnection);
}

void DeviceWorker::Slot_StartWorker(QThread* _parentthread,int _userid, int _deviceid, QString _hostip, quint16 _hostport, int _delayconnect, int _delayread, unsigned long _delayrelay, QString _ip, long _port)
{
    if(ResolveFunctions()){
        qDebug() << _ip << " : " << "thread id " << QThread::currentThreadId() << " can resolve functions";
        qDebug() << _ip << " : " << "thread id " << QThread::currentThreadId() << " connecting to " << _hostip << ":" << _hostport;

        qDebug() << _ip << " : " << "closing door";
        int handler = -1;
        handler = ConnectToDevice(_deviceid,_ip.toLocal8Bit().data(),_port,5000,0,0,1262);
        if(handler>0)
        {
            qDebug() << _ip << " : " << "can connect to device";
            int ret = -1;
            ret = CloseDeviceDoor(handler);
            if(ret == 1){
                qDebug() << _ip << " : " << "can close door";
            }
            else{
                qDebug() << _ip << " : " << "can not close door";
            }
            DisconnectFromDevice(handler);
        }
        else{
            qDebug() << _ip << " : " << "can not connect to device";
            emit this->Sig_Quit();
        }



        myTcpSocket = new QTcpSocket(this);
        myTcpSocket->connectToHost(_hostip,_hostport,QIODevice::WriteOnly,QAbstractSocket::IPv4Protocol);
        if (myTcpSocket->waitForConnected(_delayconnect)){
            qDebug() << _ip << " : " << "thread id " << QThread::currentThreadId() << " can connect access server";
            QString pm = QString::number(_userid) + "," + QString::number(_deviceid);
            qDebug() << _ip << " : " << "pm = " << pm;
            qDebug() << _ip << " : " << "send to server response : " << myTcpSocket->write(pm.toLocal8Bit().data());
            qDebug() << _ip << " : " << "flush : " << myTcpSocket->flush();
            QThread::msleep(_delayrelay);
//            if(myTcpSocket->waitForReadyRead(_delayread)){
//                qDebug() << "thread id " << QThread::currentThreadId() << " can received pm from access server";
//                QString resp(myTcpSocket->readAll());
//                qDebug() << "thread id " << QThread::currentThreadId() << " received pm from access server for device user id " << _userid << " and device id " << _deviceid << " is :" << resp;
//                if(resp.toInt() == 0){
//                    qDebug() << " thread id " << QThread::currentThreadId() << " open the door";
//                    qDebug() << _ip;
//                    qDebug() << _port;
//                    qDebug() << _deviceid;
//                    int handler = ConnectToDevice(_deviceid,_ip.toLocal8Bit().data(),_port,5000,0,0,1262);
//                        OpenDeviceDoor(handler);
//                        QThread::msleep(_delayrelay);
//                        CloseDeviceDoor(handler);
//                        DisconnectFromDevice(handler);
//                }
//                else if(resp.toInt() == 1){
//                    qDebug() << " thread id " << QThread::currentThreadId() << " close the door";
//                    qDebug() << _ip;
//                    qDebug() << _port;
//                    qDebug() << _deviceid;
//                    int handler = ConnectToDevice(_deviceid,_ip.toLocal8Bit().data(),_port,5000,0,0,1262);
//                    CloseDeviceDoor(handler);
//                    DisconnectFromDevice(handler);
//                }
//                else{
//                    qDebug() << " thread id " << QThread::currentThreadId() << "                             wrong pm";
//                }
//                qDebug() << "thread id " << QThread::currentThreadId() << " finish";
//                myTcpSocket->disconnectFromHost();
//                myTcpSocket->close();
//                myTcpSocket->deleteLater();
//                this->deleteLater();
//                qDebug() << "delete thread";
//                qDebug() << "emit thread id " << QThread::currentThreadId() << " deleting ";
//                emit this->Sig_FinishWorker(this,_parentthread);
//            }
//            else{
//                qDebug() << "thread id " << QThread::currentThreadId() << " can not received pm from access server";
//                myTcpSocket->disconnectFromHost();
//                myTcpSocket->close();
//                myTcpSocket->deleteLater();
//                this->deleteLater();
//                qDebug() << "delete thread";
//                qDebug() << "emit thread id " << QThread::currentThreadId() << " deleting ";
//                emit this->Sig_FinishWorker(this,_parentthread);
//            }

            qDebug() << _ip << " : " << "thread id " << QThread::currentThreadId() << " can send data to access server";
            qDebug() << _ip << " : " << "emit thread id " << QThread::currentThreadId() << " deleting ";

            myTcpSocket->disconnectFromHost();
            if (myTcpSocket->state() == QAbstractSocket::UnconnectedState
                || myTcpSocket->waitForDisconnected(1000)) {
                    qDebug() << _ip << " : " << "Disconnected !";
            }
//            myTcpSocket->close();
//            myTcpSocket->abort();
//            myTcpSocket->deleteLater();
//            this->deleteLater();
            qDebug() << _ip << " : " << "delete thread";
//            emit this->Sig_FinishWorker(this,_parentthread);
        }
        else{

            qDebug() << _ip << " : " << "thread id " << QThread::currentThreadId() << " can not connect access server";
            qDebug() << _ip << " : " << "emit thread id " << QThread::currentThreadId() << " deleting ";
            myTcpSocket->disconnectFromHost();
            if (myTcpSocket->state() == QAbstractSocket::UnconnectedState
                || myTcpSocket->waitForDisconnected(1000)) {
                    qDebug() << _ip << " : " << "Disconnected !";
            }
//            myTcpSocket->close();
//            myTcpSocket->abort();
//            myTcpSocket->deleteLater();
//            this->deleteLater();
            qDebug() << _ip << " : " << "delete thread";
//            emit this->Sig_FinishWorker(this,_parentthread);
        }
    }
    else{
        qDebug() << _ip << " : " << "thread id " << QThread::currentThreadId() << " can not resolve functions";
        qDebug() << _ip << " : " << "emit thread id " << QThread::currentThreadId() << " deleting ";
//        this->deleteLater();
        qDebug() << _ip << " : " << "delete thread";
//        emit this->Sig_FinishWorker(this,_parentthread);
    }
}

bool DeviceWorker::ResolveFunctions()
{
    bool ret = true;
    if (QLibrary::isLibrary("./FKAttend_old.dll")) { // FKAttend_old.dll
        QLibrary lib("./FKAttend_old.dll");
        if (!lib.load()) {
//            qDebug() << lib.errorString();
            ret = false;
        }
        else{
            qDebug() << "success load lib in thread id " << QThread::currentThreadId();
            FK_ConnectNet = (ConnectNet)lib.resolve("FK_ConnectNet");
            if (FK_ConnectNet){
//                qDebug() << "FK_ConnectNet Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_ConnectNet Can Not resolve : " << lib.errorString();
            }


            FK_DisConnect = (DisConnect)lib.resolve("FK_DisConnect");
            if (FK_DisConnect){
//                qDebug() << "FK_DisConnect Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_DisConnect Can Not resolve : " << lib.errorString();
            }


            FK_EmptyGeneralLogData = (EmptyGeneralLogData)lib.resolve("FK_EmptyGeneralLogData");
            if (FK_EmptyGeneralLogData){
//                qDebug() << "FK_EmptyGeneralLogData Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_EmptyGeneralLogData Can Not resolve : " << lib.errorString();
            }

            FK_SetDoorStatus = (SetDoorStatus)lib.resolve("FK_SetDoorStatus");
            if (FK_SetDoorStatus){
//                qDebug() << "FK_SetDoorStatus Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_SetDoorStatus Can Not resolve : " << lib.errorString();
            }

            FK_EnableDevice = (EnableDevice)lib.resolve("FK_EnableDevice");
            if (FK_EnableDevice){
//                qDebug() << "FK_EnableDevice Can resolve ";
            }
            else {
                ret = false;
                qDebug() << "FK_EnableDevice Can Not resolve : " << lib.errorString();
            }
        }
    }
    else{
        ret = false;
    }
    return ret;
}

int DeviceWorker::ConnectToDevice(int _deviceid, QString _deviceip, int _deviceport, int _devicetimeout, int _deviceprotocol, int _devicepass, int _devicelicense)
{
    int handler = FK_ConnectNet(_deviceid,_deviceip.toLocal8Bit().data(),_deviceport,_devicetimeout,_deviceprotocol,_devicepass,_devicelicense);
    return handler;
}

bool DeviceWorker::DisconnectFromDevice(int _devicehandler)
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

bool DeviceWorker::OpenDeviceDoor(int _devicehandler)
{
    if(_devicehandler > 0){
        int ret = FK_SetDoorStatus(_devicehandler,3);
        if(ret == 1)
            return true;
        else
            return false;
    }
    else
        return false;

}

bool DeviceWorker::CloseDeviceDoor(int _devicehandler)
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

bool DeviceWorker::ClearDeviceLog(int _devicehandler)
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
