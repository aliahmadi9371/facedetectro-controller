#-------------------------------------------------
#
# Project created by QtCreator 2020-08-23T20:11:32
#
#-------------------------------------------------

QT       += core gui axcontainer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceRealtimeServer
TEMPLATE = app
RC_FILE = myapp.rc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    realsvrocxtcplib.cpp \
    deviceworker.cpp

HEADERS += \
        mainwindow.h \
    realsvrocxtcplib.h \
    deviceworker.h \
    globals.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

