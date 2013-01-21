
QT += core gui svg

TARGET = QtRenderer

TEMPLATE = lib
CONFIG      += plugin
INCLUDEPATH += ../ViewRenderer \
    ../ChartProviderInterface \
    ../UpWindScene \
    ../UpWindScene/Scene
DEPENDPATH += ../ViewRenderer \
    ../ChartProviderInterface \
    ../UpWindScene \
    ../UpWindScene/Scene

#LIBS += ../UpWindScene/route.o \
#../UpWindScene/naviglineinfo.o

HEADERS += \
    qtrenderer.h \
    chartwidget.h \
    routewidget.h \
    ../ViewRenderer/coreviewrenderer.h \
    ../ViewRenderer/viewrendererinterface.h \
    ../UpWindScene/Scene/boat.h \
    ../shared/uwmath.h \
    boatwidget.h \
    ../shared/uwmath.h
    #../ChartProviderInterface/corechartprovider.h \
    #../ChartProviderInterface/objects/coastline.h \
    #../ChartProviderInterface/objects/deptharea.h \
    #../ChartProviderInterface/objects/rock.h \
    #../ChartProviderInterface/objects/wreck.h \
    #../ChartProviderInterface/objects/limit.h \
HEADERS += ../UpWindScene/Scene/naviglineinfo.h \
           ../UpWindScene/Scene/route.h \
#           ../UpWindScene/Scene/shortnavig.h \
    ../UpWindScene/Scene/polardiagram.h



SOURCES += \
    qtrenderer.cpp \
    chartwidget.cpp \
    routewidget.cpp \
    ../ViewRenderer/coreviewrenderer.cpp \
    ../UpWindScene/Scene/boat.cpp \
    ../shared/uwmath.cpp \
    boatwidget.cpp
    #../ChartProviderInterface/objects/coastline.cpp \
    #../ChartProviderInterface/objects/deptharea.cpp \
    #../ChartProviderInterface/objects/rock.cpp \
    #../ChartProviderInterface/objects/wreck.cpp \
    #../ChartProviderInterface/objects/limit.cpp \
SOURCES +=  ../UpWindScene/Scene/naviglineinfo.cpp \
           ../UpWindScene/Scene/route.cpp \
#           ../UpWindScene/Scene/shortnavig.cpp \
        ../UpWindScene/Scene/polardiagram.cpp


DESTDIR = ../plugins


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



