
QT       += core widgets svg testlib
CONFIG += testcase

SOURCES += \
    Scene/calculatelaylines_test.cpp

HEADERS += \
    Scene/calculatelaylines_test.h

    INCLUDEPATH += /usr/include/postgresql
    LIBS += -L../plugins -lUpWindScene
    LIBS +=-lpq -lgdal
