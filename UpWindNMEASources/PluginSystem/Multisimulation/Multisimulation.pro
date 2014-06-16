#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T12:08:41
#
#-------------------------------------------------

#TODO: Check the needed Qt kits by yourself before compiling

TEMPLATE    =   lib
QT       += core widgets \
            webkitwidgets xml network
CONFIG      +=  plugin

DESTDIR     = ../../Release/plugins
TARGET      =   $$qtLibraryTarget(Multisimulation)

INCLUDEPATH += ../CorePlugin \
               ../PluginController \
               #../NMEAGPS \
               ../NMEA_Anemometer \
               ../../UpWindNMEA/src \
               ../../UpWindNMEA/PluginController

SOURCES +=\
    multisimulation.cpp
    #../CorePlugin/coreplugin.cpp \
    #../../UpWindNMEA/src/server.cpp \
    #../../UpWindNMEA/src/plugincontroller.cpp \
    #../../UpWindNMEA/src/xmlcontroller.cpp \
    #../../UpWindNMEA/src/serialsrc/serialport.cpp \
    #../../UpWindNMEA/src/serialsrc/rs232.c \
    #../../UpWindNMEA/src/filecontroller.cpp \
    #../../UpWindNMEA/src/savingtype.cpp \
    #../../UpWindNMEA/src/client.cpp \
    #../../UpWindNMEA/src/boat.cpp \
    #exportplug.cpp \
    #../NMEA_Anemometer/nmea_anemometer.cpp\
    #../../UpWindNMEA/PluginController/qeffects.cpp

HEADERS  += \
    multisimulation.h \
    #../CorePlugin/coreplugin.h \
    #../../UpWindNMEA/src/server.h \
    #../../UpWindNMEA/src/plugincontroller.h \
    #../../UpWindNMEA/src/xmlcontroller.h \
    #../../UpWindNMEA/src/serialsrc/serialport.h \
    #../../UpWindNMEA/src/serialsrc/rs232.h \
    #../../UpWindNMEA/src/delaytimer.h \
    #../../UpWindNMEA/src/filecontroller.h \
    #../../UpWindNMEA/src/savingtype.h \
    #../../UpWindNMEA/src/client.h \
    #../../UpWindNMEA/src/boat.h \
    #../NMEA_Anemometer/nmea_anemometer.h \
    #nmea_anemometer.h \
    #../../UpWindNMEA/PluginController/qeffects_p.h

HEADERS += ../../UpWindNMEA/src/settingsmanager.h
#SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

FORMS    += multiwidget.ui ../NMEA_Anemometer/anemometerwidget.ui

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS Multisimulation.pro
sources.path = ./
INSTALLS += target sources
