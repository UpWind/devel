QT += widgets xml
TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH    += ../CorePlugin
HEADERS         = nmeaclock.h \
    ../CorePlugin/coreplugin.h
SOURCES         = nmeaclock.cpp \
    ../CorePlugin/coreplugin.cpp \
    exportplug.cpp
TARGET          = $$qtLibraryTarget(nmeaclock)
DESTDIR     = ../../Release/plugins

INCLUDEPATH += ../../UpWindNMEA/src
HEADERS += ../../UpWindNMEA/src/settingsmanager.h
SOURCES += ../../UpWindNMEA/src/settingsmanager.cpp

MOC_DIR = mocs
OBJECTS_DIR = objects
UI_DIR = uis

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS NMEAClock.pro
sources.path = ./
INSTALLS += target sources

FORMS += \
    clockwidget.ui
