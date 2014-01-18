#-------------------------------------------------
#
# Project created by QtCreator 2011-02-10T09:17:02
#
#-------------------------------------------------

QT += core widgets\
      network \
      xml\
      webkit


TARGET = ViewRenderer
TEMPLATE = lib

INCLUDEPATH += ../ChartProviderInterface

SOURCES += \
    coreviewrenderer.cpp \
    ../../Settings/settings.cpp

HEADERS += \
    coreviewrenderer.h \
    ../../Settings/settings.h \
    viewrendererinterface.h

DESTDIR = ../plugins


