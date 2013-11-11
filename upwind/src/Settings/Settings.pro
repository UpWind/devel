#-------------------------------------------------
#
# Project created by QtCreator 2011-02-24T09:11:20
#
#-------------------------------------------------
CONFIG += serialport
QT += serialport
QT += widgets

QT       += core gui xml

TARGET = Settings
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    settings.h

FORMS    += mainwindow.ui
