
QT += core widgets svg opengl

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
    routewidget.h \
    ../ViewRenderer/coreviewrenderer.h \
    ../ViewRenderer/viewrendererinterface.h \
    ../UpWindScene/Scene/boat.h \
    ../shared/uwmath.h \
    boatwidget.h \
    ../shared/uwmath.h \
    chartgraphicsobject.h \
    chartview.h
    #../ChartProviderInterface/corechartprovider.h \
    #../ChartProviderInterface/objects/coastline.h \
    #../ChartProviderInterface/objects/deptharea.h \
    #../ChartProviderInterface/objects/rock.h \
    #../ChartProviderInterface/objects/wreck.h \
    #../ChartProviderInterface/objects/limit.h \
HEADERS += ../UpWindScene/Scene/naviglineinfo.h \
           ../UpWindScene/Scene/route.h \
    ../UpWindScene/Scene/polardiagram.h



SOURCES += \
    qtrenderer.cpp \
    routewidget.cpp \
    ../ViewRenderer/coreviewrenderer.cpp \
    ../UpWindScene/Scene/boat.cpp \
    ../shared/uwmath.cpp \
    boatwidget.cpp \
    chartgraphicsobject.cpp \
    chartview.cpp
    #../ChartProviderInterface/objects/coastline.cpp \
    #../ChartProviderInterface/objects/deptharea.cpp \
    #../ChartProviderInterface/objects/rock.cpp \
    #../ChartProviderInterface/objects/wreck.cpp \
    #../ChartProviderInterface/objects/limit.cpp \
SOURCES +=  ../UpWindScene/Scene/naviglineinfo.cpp \
           ../UpWindScene/Scene/route.cpp \
        ../UpWindScene/Scene/polardiagram.cpp


DESTDIR = ../plugins


mac {
    INCLUDEPATH += /Library/Frameworks/GDAL.framework/Headers/
    LIBS += -L/Library/Frameworks/GDAL.framework/unix/lib -lgdal -lpq
}

else:unix {

    INCLUDEPATH +=/usr/include/gdal/ \
        /usr/include/postgresql
    LIBS +=-lpq -lgdal
}

win32 {
    LIBS += -LC:\libs\lib -lpq -lgdal-1
    INCLUDEPATH += "C:\libs\include"
}



