#-------------------------------------------------
#
# Project created by QtCreator 2011-02-22T13:00:09
#
#-------------------------------------------------

QT       += core gui xml

TARGET = Compass

TEMPLATE = lib


SOURCES +=\
        compass.cpp \
    ../CoreNMEAInstrument/corenmeainstrument.cpp \
    ../CoreNMEAInstrument/instrumentview.cpp \
    ../../../Settings/settings.cpp

HEADERS  += compass.h \
    ../CoreNMEAInstrument/corenmeainstrument.h \
    ../CoreNMEAInstrument/instrumentview.h \
    ../../../Settings/settings.h

FORMS    += compass.ui

RESOURCES += \
    resources.qrc

DESTDIR         = ../../plugins
