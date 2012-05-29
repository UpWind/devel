

QT       += core gui xml

TARGET = GPS
TEMPLATE = lib


HEADERS += \
    gps.h \
    ../CoreNMEAInstrument/corenmeainstrument.h \
    ../CoreNMEAInstrument/instrumentview.h \
    ../../../Settings/settings.h

SOURCES += \
    gps.cpp \
    ../CoreNMEAInstrument/corenmeainstrument.cpp \
    ../CoreNMEAInstrument/instrumentview.cpp \
    ../../../Settings/settings.cpp

FORMS += \
    gps.ui

RESOURCES += \
    resource.qrc


DESTDIR         = ../../plugins
