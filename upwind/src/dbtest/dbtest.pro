#-------------------------------------------------
#
# Project created by QtCreator 2011-11-03T13:57:31
#
#-------------------------------------------------

QT       += core gui

TARGET = dbtest
TEMPLATE = app


SOURCES += main.cpp \
    coastline.cpp

HEADERS  += \
    coastline.h

INCLUDEPATH += ../UWPlugins

LIBS     += -L ../UWPlugins/plugins/ -lPostgreChartProvider





