#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T11:46:33
#
#-------------------------------------------------

TARGET = UpWindScene
TEMPLATE = lib
DESTDIR = ../plugins

QT       += core widgets\
            network \
            xml\
            webkit\
            svg

SOURCES += \
    coreupwindscene.cpp \
    Scene/boat.cpp \
    Scene/route.cpp \
    settingsui.cpp \
    ../../Settings/settings.cpp \
    Scene/polardiagram.cpp \
    Scene/naviglineinfo.cpp \
    ../shared/uwmath.cpp \
    Scene/calculatelaylines.cpp \
    Scene/loadObstacles.cpp

HEADERS += \
    coreupwindscene.h \
    UpWindSceneInterface.h \
    Scene/boat.h \
    Scene/route.h \
    Scene/projection.h \
    Scene/naviglineinfo.h \
    settingsui.h \
    ../../Settings/settings.h \
    Scene/polardiagram.h \
    ../shared/uwmath.h \
    Scene/calculatelaylines.h \
    Scene/loadObstacles.h

FORMS += \
    settingsui.ui

RESOURCES += \
    resources.qrc

mac {
    INCLUDEPATH += /Library/Frameworks/GDAL.framework/Headers/
    LIBS += -L/Library/Frameworks/GDAL.framework/unix/lib -lgdal -lpq
}

win32 {
    LIBS += -LC:\libs\lib -lpq -lgdal-1
    INCLUDEPATH += "C:\libs\include"
}

unix {
    INCLUDEPATH += /usr/include/gdal/ /usr/include/postgresql
    LIBS +=-lpq -lgdal
}
