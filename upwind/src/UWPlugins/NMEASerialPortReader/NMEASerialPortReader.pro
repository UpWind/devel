#-------------------------------------------------
#
# Project created by QtCreator 2011-02-23T11:02:16
#
#-------------------------------------------------

CONFIG += serialport
QT += serialport

QT       += core gui xml

TARGET = NMEASerialPortReader

TEMPLATE = lib


SOURCES += \
    ../NMEAReader/corenmeareader.cpp \
    serialport.cpp \
    ../../Settings/settings.cpp \
    settingsui.cpp

HEADERS += \
    ../NMEAReader/corenmeareader.h \
    ../NMEAReader/NMEAReaderInterface.h \
    serialport.h \
    ../../Settings/settings.h \
    settingsui.h \
    Device.h


DESTDIR         = ../plugins

FORMS += \
    settingsui.ui
