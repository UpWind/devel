#-------------------------------------------------
#
# Project created by QtCreator 2011-02-21T09:20:02
#
#-------------------------------------------------

QT       += core gui xml

TARGET = DataSimulator

TEMPLATE = lib


SOURCES += \
    datasimulator.cpp \
    ../NMEAReader/corenmeareader.cpp \
    settingsui.cpp \
    ../../Settings/settings.cpp

HEADERS += \
    datasimulator.h \
    ../NMEAReader/corenmeareader.h \
    ../NMEAReader/nmeareaderinterface.h \
    settingsui.h \
    ../../Settings/settings.h


DESTDIR         = ../plugins

FORMS += \
    settingsui.ui

RESOURCES +=
