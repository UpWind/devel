#-------------------------------------------------
#
# Project created by QtCreator 2010-09-29T09:11:48
#
#-------------------------------------------------

QT       += core gui network xml

TARGET = UpwindSimulator
DESTDIR = ../../Release
TEMPLATE = app

SOURCES += main.cpp\
    ../../PluginSystem/CorePlugin/coreplugin.cpp \
    controllerwindow.cpp \
    instrumentpanel.cpp \
    ../../UpWindNMEA/src/serialsrc/serialport.cpp \
    ../../UpWindNMEA/src/serialsrc/rs232.c \
    ../../UpWindNMEA/src/server.cpp \
    ../../UpWindNMEA/src/plugincontroller.cpp \
    ../../UpWindNMEA/src/savingtype.cpp \
    ../../UpWindNMEA/src/filecontroller.cpp \
    ../../UpWindNMEA/src/xmlcontroller.cpp \
    ../../UpWindNMEA/src/settingsmanager.cpp \
    ../../UpWindNMEA/src/upwindhighlighter.cpp \
    ../../UpWindNMEA/src/checkboxmodel.cpp \
    ../../UpWindNMEA/src/client.cpp \
    plugindelegate.cpp \
    qeffects.cpp

HEADERS  += \
    ../../PluginSystem/CorePlugin/coreplugin.h \
    ../../PluginSystem/CorePlugin/coreinterface.h \
    controllerwindow.h \
    instrumentpanel.h \
    ../../UpWindNMEA/src/serialsrc/serialport.h \
    ../../UpWindNMEA/src/serialsrc/rs232.h \
    ../../UpWindNMEA/src/server.h \
    ../../UpWindNMEA/src/plugincontroller.h \
    ../../UpWindNMEA/src/filecontroller.h \
    ../../UpWindNMEA/src/xmlcontroller.h \
    ../../UpWindNMEA/src/savingtype.h \
    ../../UpWindNMEA/src/delaytimer.h \
    ../../UpWindNMEA/src/settingsmanager.h \
    ../../UpWindNMEA/src/upwindhighlighter.h \
    ../../UpWindNMEA/src/checkboxmodel.h \
    ../../UpWindNMEA/src/environment.h \
    ../../UpWindNMEA/src/client.h \
    plugindelegate.h \
    qeffects_p.h

INCLUDEPATH += ../../PluginSystem/CorePlugin \
    ../../UpWindNMEA/src/ \
    ../../UpWindNMEA/

FORMS += \
    controllerwindow.ui \
    instrumentpanel.ui

RESOURCES += \
    windowresources.qrc

RC_FILE = appicon.rc
