#-------------------------------------------------
#
# Project created by QtCreator 2011-02-14T12:12:22
#
#-------------------------------------------------


QT       += core
QT       += xml
QT       += widgets

TEMPLATE = lib

TARGET = Logger

DESTDIR += ../plugins

HEADERS += \
    LoggerInterface.h \
    corelogger.h \
    nmeaentry.h \
    ../../Settings/settings.h

SOURCES += \
    corelogger.cpp \
    nmeaentry.cpp \
    ../../Settings/settings.cpp

FORMS +=

RESOURCES +=
