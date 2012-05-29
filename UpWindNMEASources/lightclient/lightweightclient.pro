# -------------------------------------------------
# Project created by QtCreator 2010-09-15T10:24:16
# -------------------------------------------------
QT += core gui network \
    xml
TARGET = Lightweight_client
DESTDIR = ../Release
TEMPLATE = app
INCLUDEPATH += ../UpWindNMEA/src \
    ../UpWindNMEA/src/serialsrc
SOURCES += main.cpp \
    mainwindow.cpp \
    ../UpWindNMEA/src/client.cpp \
    ../UpWindNMEA/src/serialsrc/serialport.cpp \
    ../UpWindNMEA/src/serialsrc/rs232.c \
    ../UpWindNMEA/src/upwindhighlighter.cpp
HEADERS += mainwindow.h \
    ../UpWindNMEA/src/client.h \
    ../UpWindNMEA/src/serialsrc/serialport.h \
    ../UpWindNMEA/src/serialsrc/rs232.h \
    ../UpWindNMEA/src/upwindhighlighter.h \
    ../UpWindNMEA/src/environment.h
FORMS += mainwindow.ui

RESOURCES += \
    tcpclientresource.qrc

RC_FILE = appicon.rc
