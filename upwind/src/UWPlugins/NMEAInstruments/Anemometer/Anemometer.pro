#-------------------------------------------------
#
# Project created by QtCreator 2011-03-02T15:33:31
#
#-------------------------------------------------

QT       += core gui xml

TARGET = Anemometer
TEMPLATE = lib


SOURCES +=\   
    anemometer.cpp \
    ../CoreNMEAInstrument/corenmeainstrument.cpp \
    ../CoreNMEAInstrument/instrumentview.cpp \
    ../../../Settings/settings.cpp

HEADERS  += anemometer.h \
    ../CoreNMEAInstrument/corenmeainstrument.h \
    ../CoreNMEAInstrument/nmeainstrumentinterface.h \
    ../CoreNMEAInstrument/instrumentview.h \
    ../../../Settings/settings.h

FORMS    += \
    anemometer.ui

DESTDIR         = ../../plugins

RESOURCES += \
    resources.qrc
