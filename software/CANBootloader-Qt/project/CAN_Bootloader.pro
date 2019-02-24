#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T09:26:29
#
#-------------------------------------------------

QT       += core gui

TARGET = USB2CANBootloader
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32{
    LIBS += -L$$PWD/lib/win32/ -lUSB2XXX
}else{
    unix:contains(QMAKE_HOST.arch, x86_64){
        LIBS += -L$$PWD/lib/linux64/ -lUSB2XXX
    }else{
        LIBS += -L$$PWD/lib/linux32/ -lUSB2XXX
    }
}
!win32: DEFINES += OS_UNIX

#DEFINES += LANGUE_EN

RESOURCES += \
    ../source/image.qrc
RC_FILE = \
    ../source/ico.rc

FORMS += \
    ../source/mainwindow_ch.ui \
    ../source/mainwindow_en.ui \
    ../source/scandevrangedialog.ui \
    ../source/dialogaddnode.ui

OTHER_FILES += \
    ../source/ico.rc

HEADERS += \
    ../source/mainwindow.h \
    ../source/usb_device.h \
    ../source/usb2can.h \
    ../source/scandevrangedialog.h \
    ../source/dialogaddnode.h

SOURCES += \
    ../source/main.cpp \
    ../source/mainwindow.cpp \
    ../source/scandevrangedialog.cpp \
    ../source/dialogaddnode.cpp
