#-------------------------------------------------
#
# Project created by QtCreator 2011-03-16T10:55:42
#
#-------------------------------------------------

QT       += core
QT       += xml
QT       += gui

TEMPLATE = lib


TARGET = NMEALogger

DESTDIR += ../plugins

SOURCES += \
    nmealogger.cpp \
    settingsui.cpp \
    ../Logger/corelogger.cpp \
    ../../Settings/settings.cpp \
    ../Logger/nmeaentry.cpp

HEADERS += \
    ../Logger/LoggerInterface.h \
    ../Logger/corelogger.h \
    ../Logger/nmeaentry.h \
    nmealogger.h \
    settingsui.h \
    ../../Settings/settings.h

FORMS += \
    settingsui.ui


