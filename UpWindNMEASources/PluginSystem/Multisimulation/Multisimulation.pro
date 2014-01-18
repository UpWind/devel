#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T12:08:41
#
#-------------------------------------------------

#TODO: Check the needed Qt kits by yourself before compiling

QT       += core widgets \
            webkit xml network

TARGET      =   $$qtLibraryTarget(Multisimulation)
TEMPLATE    =   lib
CONFIG      +=  plugin
INCLUDEPATH += ../CorePlugin
INCLUDEPATH += ../PluginController \
    ../NMEAGPS/ \
    ../NMEA_Anemometer/
DESTDIR     = ../../Release/plugins

SOURCES +=\
    ../CorePlugin/coreplugin.cpp \
    multisimulation.cpp \
    ../../UpWindNMEA/src/server.cpp \
    ../../UpWindNMEA/src/plugincontroller.cpp \
    ../../UpWindNMEA/src/xmlcontroller.cpp \
    ../../UpWindNMEA/src/serialsrc/serialport.cpp \
    ../../UpWindNMEA/src/serialsrc/rs232.c \
    ../../UpWindNMEA/src/filecontroller.cpp \
    ../../UpWindNMEA/src/savingtype.cpp \
    ../../UpWindNMEA/src/client.cpp \
    ../../UpWindNMEA/src/boat.cpp \
    exportplug.cpp \
    ../NMEA_Anemometer/nmea_anemometer.cpp\
    ../../UpWindNMEA/PluginController/qeffects.cpp

HEADERS  += \
    ../CorePlugin/coreplugin.h \
    multisimulation.h \
    ../../UpWindNMEA/src/server.h \
    ../../UpWindNMEA/src/plugincontroller.h \
    ../../UpWindNMEA/src/xmlcontroller.h \
    ../../UpWindNMEA/src/serialsrc/serialport.h \
    ../../UpWindNMEA/src/serialsrc/rs232.h \
    ../../UpWindNMEA/src/delaytimer.h \
    ../../UpWindNMEA/src/filecontroller.h \
    ../../UpWindNMEA/src/savingtype.h \
    ../../UpWindNMEA/src/client.h \
    ../../UpWindNMEA/src/boat.h \
    ../NMEA_Anemometer/nmea_anemometer.h \
    ../../UpWindNMEA/PluginController/qeffects_p.h

INCLUDEPATH += ../../UpWindNMEA/src \
               ../../UpWindNMEA/PluginController
HEADERS += ../../UpWindNMEA/src/settingsmanager.h
SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

FORMS    += multiwidget.ui ../NMEA_Anemometer/anemometerwidget.ui

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS Multisimulation.pro
sources.path = ./
INSTALLS += target sources
