
QT       += core widgets xml opengl

TARGET = NMEAInstrument

TEMPLATE = lib

HEADERS += \
    corenmeainstrument.h \
    ../../../Settings/settings.h \
    instrumentview.h \
    nmeainstrumentinterface.h

SOURCES += \
    corenmeainstrument.cpp \
    ../../../Settings/settings.cpp \
    instrumentview.cpp

DESTDIR         = ../../plugins


