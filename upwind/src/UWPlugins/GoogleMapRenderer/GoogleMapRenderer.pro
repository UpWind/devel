QT       += core widgets\
            network \
            xml\
            webkit


TEMPLATE = lib

HEADERS += \
    ../ViewRenderer/coreviewrenderer.h \
    ../ViewRenderer/viewrendererinterface.h \
    googlemaprenderer.h \
    randomnavigate.h \
    googlemapchart.h \
    ../../Settings/settings.h

SOURCES += \
    ../ViewRenderer/coreviewrenderer.cpp \
    googlemaprenderer.cpp \
    randomnavigate.cpp \
    googlemapchart.cpp \
    ../../Settings/settings.cpp

DESTDIR         = ../plugins
