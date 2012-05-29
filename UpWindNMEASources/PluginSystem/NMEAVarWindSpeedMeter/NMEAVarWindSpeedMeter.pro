#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T12:08:41
#
#-------------------------------------------------

#TODO: Check the needed Qt kits by yourself before compiling

QT       += core gui \
            webkit xml

TARGET      =   $$qtLibraryTarget(NMEAVarWindSpeedMeter)
TEMPLATE    =   lib
CONFIG      +=  plugin
INCLUDEPATH += ../CorePlugin
DESTDIR     = ../../Release/plugins

SOURCES +=\
    ../CorePlugin/coreplugin.cpp \
    nmeavarwindspeedmeter.cpp \
    exportplug.cpp \
    mainwindow.cpp \
    mygraphicsview.cpp \
    myline.cpp

HEADERS  += \
    ../CorePlugin/coreplugin.h \
    nmeavarwindspeedmeter.h \
    mainwindow.h \
    mygraphicsview.h \
    myline.h

INCLUDEPATH += ../../UpWindNMEA/src
HEADERS += ../../UpWindNMEA/src/settingsmanager.h
SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

FORMS    += nmeavarwindspeedmeter.ui \
    dialog.ui \
    mainwindow.ui

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS NMEAVarWindSpeedMeter.pro
sources.path = ./
INSTALLS += target sources

