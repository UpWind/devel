#-------------------------------------------------
#
# Project created by QtCreator 2011-02-23T11:02:16
#
#-------------------------------------------------

CONFIG += serialport
QT += serialport

QT       += core widgets xml

TARGET = NMEASerialPortReader

TEMPLATE = lib


SOURCES += \
    serialportreader.cpp \
    ../NMEAReader/corenmeareader.cpp \
    ../../Settings/settings.cpp \
    settingsui.cpp

HEADERS += \
    serialportreader.h \
    ../NMEAReader/corenmeareader.h \
    ../NMEAReader/NMEAReaderInterface.h \
    ../../Settings/settings.h \
    settingsui.h \
    Device.h


DESTDIR         = ../plugins

FORMS += \
    settingsui.ui
