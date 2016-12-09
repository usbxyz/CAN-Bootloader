#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T09:26:29
#
#-------------------------------------------------

QT       += core gui

TARGET = USB2CANBootloader
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:LIBS += -L$$PWD/lib/win32/ -lUSB2XXX

//DEFINES += LANGUE_EN

RESOURCES += \
    ../source/image.qrc
RC_FILE = \
    ../source/ico.rc

FORMS += \
    ../source/mainwindow_ch.ui \
    ../source/mainwindow_en.ui \
    ../source/scandevrangedialog.ui

OTHER_FILES += \
    ../source/ico.rc

HEADERS += \
    ../source/CANBootloader.h \
    ../source/mainwindow.h \
    ../source/usb_device.h \
    ../source/usb2can.h \
    ../source/scandevrangedialog.h

SOURCES += \
    ../source/main.cpp \
    ../source/mainwindow.cpp \
    ../source/scandevrangedialog.cpp
