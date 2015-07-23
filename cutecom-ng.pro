#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:52:08
#
#-------------------------------------------------

QT       += core gui serialport uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cutecom-ng
TEMPLATE = app

INCLUDEPATH += libs

SOURCES += main.cpp\
        mainwindow.cpp \
    connectdialog.cpp \
    sessionmanager.cpp \
    outputmanager.cpp \
    historycombobox.cpp \
    history.cpp \
    searchhighlighter.cpp \
    xmodemtransfer.cpp \
    filetransfer.cpp \
    libs/crc16.cpp \
    libs/xmodem.cpp \
    libs/fymodem.cpp \
    ymodemtransfer.cpp

HEADERS  += mainwindow.h \
    connectdialog.h \
    sessionmanager.h \
    outputmanager.h \
    historycombobox.h \
    history.h \
    searchhighlighter.h \
    xmodemtransfer.h \
    filetransfer.h \
    libs/crc16.h \
    libs/xmodem.h \
    libs/fymodem.h \
    ymodemtransfer.h

FORMS    += mainwindow.ui \
    connectdialog.ui \
    searchwidget.ui

RESOURCES += \
    cutecom-ng.qrc
