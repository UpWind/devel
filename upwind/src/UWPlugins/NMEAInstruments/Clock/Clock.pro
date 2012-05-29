QT       += core gui xml

TARGET = Clock
TEMPLATE = lib

HEADERS += \
    clock.h \
    ../CoreNMEAInstrument/corenmeainstrument.h \
    ../CoreNMEAInstrument/instrumentview.h \
    ../../../Settings/settings.h

SOURCES += \
    clock.cpp \
    ../CoreNMEAInstrument/corenmeainstrument.cpp \
    ../CoreNMEAInstrument/instrumentview.cpp \
    ../../../Settings/settings.cpp

FORMS += \
    clock.ui

DESTDIR         = ../../plugins
