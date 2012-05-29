#-------------------------------------------------
#
# Project created by QtCreator 2010-09-29T10:14:36
#
#-------------------------------------------------

QT       += core gui \
            network \
            webkit \
            xml

TARGET      =   $$qtLibraryTarget(nmeagps)
TEMPLATE    =   lib
CONFIG      +=  plugin
INCLUDEPATH += ../CorePlugin \
               ../../UpWindNMEA/src

DESTDIR     = ../../Release/plugins

SOURCES     += nmeagps.cpp \
 ../CorePlugin/coreplugin.cpp \
    ../../UpWindNMEA/src/boat.cpp \
    ../../UpWindNMEA/src/dms.cpp \
    exportplug.cpp

HEADERS     += nmeagps.h \
 ../CorePlugin/coreplugin.h \
    ../../UpWindNMEA/src/boat.h \
    ../../UpWindNMEA/src/dms.h

OTHER_FILES += \
    ../../UpWindNMEASerial/index.html

INCLUDEPATH += ../../UpWindNMEA/src
HEADERS += ../../UpWindNMEA/src/settingsmanager.h
SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS NMEAGPS.pro
sources.path = ./
INSTALLS += target sources

FORMS += \
    simulator.ui
