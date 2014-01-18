#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T12:08:41
#
#-------------------------------------------------

#TODO: Check the needed Qt kits by yourself before compiling

QT       += core widgets \
            webkit xml

TARGET      =   $$qtLibraryTarget(RPS)
TEMPLATE    =   lib
CONFIG      +=  plugin
INCLUDEPATH += ../CorePlugin
DESTDIR     = ../../Release/plugins

SOURCES +=\
    ../CorePlugin/coreplugin.cpp \
    rps.cpp \
    exportplug.cpp

HEADERS  += \
    ../CorePlugin/coreplugin.h \
    rps.h

INCLUDEPATH += ../../UpWindNMEA/src
HEADERS += ../../UpWindNMEA/src/settingsmanager.h
SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

FORMS    += widget.ui

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS RPS.pro
sources.path = ./
INSTALLS += target sources
