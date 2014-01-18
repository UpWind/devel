# -------------------------------------------------
# Project created by QtCreator 2010-09-15T10:24:16
# -------------------------------------------------
QT += core widgets network \
    xml webkit
TARGET = HeavyClient
DESTDIR = ../Release
TEMPLATE = app
INCLUDEPATH += ../UpWindNMEA/src \
    ../UpWindNMEA/src/serialsrc \
    ../PluginSystem/CorePlugin \
    ../UpWindNMEA/PluginController \
    ../PluginSystem/NMEACompass/
SOURCES += main.cpp \
    mainwindow.cpp \
    ../UpWindNMEA/src/client.cpp \
    ../UpWindNMEA/src/serialsrc/serialport.cpp \
    ../UpWindNMEA/src/serialsrc/rs232.c \
    ../UpWindNMEA/src/upwindhighlighter.cpp \
    ../UpWindNMEA/src/plugincontroller.cpp \
    ../PluginSystem/CorePlugin/coreplugin.cpp \
    ../UpWindNMEA/PluginController/qeffects.cpp \
    ../UpWindNMEA/src/xmlcontroller.cpp \
    ../UpWindNMEA/src/filecontroller.cpp \
    ../UpWindNMEA/src/server.cpp \
    ../UpWindNMEA/src/savingtype.cpp \
    ../UpWindNMEA/src/settingsmanager.cpp \
    ../UpWindNMEA/src/boat.cpp \
    ../PluginSystem/NMEACompass/nmeacompass.cpp
HEADERS += mainwindow.h \
    ../UpWindNMEA/src/client.h \
    ../UpWindNMEA/src/serialsrc/serialport.h \
    ../UpWindNMEA/src/serialsrc/rs232.h \
    ../UpWindNMEA/src/upwindhighlighter.h \
    ../UpWindNMEA/src/environment.h \
    ../UpWindNMEA/src/plugincontroller.h \
    ../PluginSystem/CorePlugin/coreplugin.h \
    ../UpWindNMEA/PluginController/qeffects_p.h \
    ../UpWindNMEA/src/xmlcontroller.h \
    ../UpWindNMEA/src/filecontroller.h \
    ../UpWindNMEA/src/delaytimer.h \
    ../UpWindNMEA/src/server.h \
    ../UpWindNMEA/src/savingtype.h \
    ../UpWindNMEA/src/settingsmanager.h \
    ../UpWindNMEA/src/boat.h \
    ../PluginSystem/NMEACompass/nmeacompass.h
FORMS += mainwindow.ui \
    ../PluginSystem/NMEAGPS/simulator.ui \
    ../PluginSystem/NMEACompass/compasswidget.ui

RESOURCES += \
    tcpclientresource.qrc

RC_FILE = appicon.rc
