QT += xml

TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH    += ./
#DESTDIR     = ../../Release/plugins


HEADERS         = coreplugin.h \
    coreinterface.h
SOURCES         = coreplugin.cpp

target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS CorePlugin.pro
sources.path = ./
