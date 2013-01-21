#-------------------------------------------------
#
# Project created by QtCreator 2011-02-15T11:46:33
#
#-------------------------------------------------


QT       += core gui\
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
    Scene/shortnavig.cpp \
    ../shared/uwmath.cpp \
    Scene/calculatelaylines.cpp



TARGET = UpWindScene
TEMPLATE = lib

DESTDIR = ../plugins

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
    Scene/shortnavig.h \
    ../shared/uwmath.h \
    Scene/calculatelaylines.h





FORMS += \
    settingsui.ui

RESOURCES += \
    resources.qrc


mac {
    INCLUDEPATH += /Library/Frameworks/GDAL.framework/Headers/
    LIBS += -L/Library/Frameworks/GDAL.framework/unix/lib -lgdal -lpq
}

else:unix {

    INCLUDEPATH +=/usr/include/gdal/ \
        /usr/include/postgresql
    LIBS +=-lpq -lgdal1.7.0
}

win32 {
    INCLUDEPATH += "C:\\gdal\\gdal\\include"
    LIBS += "C:\\gdal\\gdal\\bin\\libgdal-1.dll"

    INCLUDEPATH += "C:\\Program Files (x86)\\PostgreSQL\\8.4\\include"
    LIBS += "C:\\Program Files (x86)\\PostgreSQL\\8.4\\bin\\libpq.dll"
}



