#-------------------------------------------------
#
# Project created by QtCreator 2010-09-28T12:35:47
#
#-------------------------------------------------

QT += xml
TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH    += ../CorePlugin
HEADERS         = nmea_anemometer.h \
    ../CorePlugin/coreplugin.h
SOURCES         = nmea_anemometer.cpp \
    ../CorePlugin/coreplugin.cpp \
    exportplug.cpp
TARGET          = $$qtLibraryTarget(nmea_anemometer)
DESTDIR     = ../../Release/plugins

INCLUDEPATH += ../../UpWindNMEA/src
HEADERS += ../../UpWindNMEA/src/settingsmanager.h
SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS NMEA_Anemometer.pro
sources.path = ./
INSTALLS += target sources

FORMS += \
    anemometerwidget.ui
