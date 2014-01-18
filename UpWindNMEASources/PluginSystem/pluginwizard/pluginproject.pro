#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T12:08:41
#
#-------------------------------------------------

#TODO: Check the needed Qt kits by yourself before compiling

QT       += core widgets \
            webkit xml

TARGET      =   $$qtLibraryTarget(%ProjectName%)
TEMPLATE    =   lib
CONFIG      +=  plugin
INCLUDEPATH += ../CorePlugin
DESTDIR     = ../../Release/plugins

SOURCES +=\
    ../CorePlugin/coreplugin.cpp \
    %ProjectName:l%.cpp \
    exportplug.cpp

HEADERS  += \
    ../CorePlugin/coreplugin.h \
    %ProjectName:l%.h

INCLUDEPATH += ../../UpWindNMEA/src
HEADERS += ../../UpWindNMEA/src/settingsmanager.h
SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

FORMS    += %ProjectName:l%.ui

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS %ProjectName%.pro
sources.path = ./
INSTALLS += target sources
