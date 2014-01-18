#-------------------------------------------------
#
# Project created by QtCreator 2011-03-02T15:33:31
#
#-------------------------------------------------

QT       += core widgets xml opengl

TARGET = VelocityHandle
TEMPLATE = lib


SOURCES +=\   
    velocityhandle.cpp \
    ../CoreNMEAInstrument/corenmeainstrument.cpp \
    ../CoreNMEAInstrument/instrumentview.cpp \
    ../../../Settings/settings.cpp \
    ../../shared/uwmath.cpp

HEADERS  += velocityhandle.h \
    ../CoreNMEAInstrument/corenmeainstrument.h \
    ../CoreNMEAInstrument/nmeainstrumentinterface.h \
    ../CoreNMEAInstrument/instrumentview.h \
    ../../../Settings/settings.h \
    ../../NMEADataSimulator/datasimulatorcontrolinterface.h \
    velocityhandleinterface.h

FORMS    += \
    velocityhandle.ui

DESTDIR         = ../../plugins

RESOURCES += \
    resources.qrc
