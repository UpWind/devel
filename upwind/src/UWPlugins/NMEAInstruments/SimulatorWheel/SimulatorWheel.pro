#-------------------------------------------------
#
# Project created by QtCreator 2011-03-02T15:33:31
#
#-------------------------------------------------

QT       += core gui xml opengl

TARGET = SimulatorWheel
TEMPLATE = lib


SOURCES +=\   
    simulatorwheel.cpp \
    ../CoreNMEAInstrument/corenmeainstrument.cpp \
    ../CoreNMEAInstrument/instrumentview.cpp \
    ../../../Settings/settings.cpp \
    ../../shared/uwmath.cpp

HEADERS  += simulatorwheel.h \
    ../CoreNMEAInstrument/corenmeainstrument.h \
    ../CoreNMEAInstrument/nmeainstrumentinterface.h \
    ../CoreNMEAInstrument/instrumentview.h \
    ../../../Settings/settings.h \
    ../../NMEADataSimulator/datasimulatorcontrolinterface.h \
    simulatorwheelinterface.h

FORMS    += \
    simulatorwheel.ui

DESTDIR         = ../../plugins

RESOURCES += \
    resources.qrc
