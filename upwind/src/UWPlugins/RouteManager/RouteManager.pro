#-------------------------------------------------
#
# Project created by QtCreator 2011-02-14T12:38:13
#
#-------------------------------------------------

QT       += core gui xml webkit svg

TARGET = RouteManager
TEMPLATE = lib

SOURCES += \
    coreroutemanager.cpp \
    ../../Settings/settings.cpp \
    route.cpp

HEADERS += \
    RouteManagerInterface.h \
    coreroutemanager.h \
    ../../Settings/settings.h \
    route.h


DESTDIR         = ../plugins
