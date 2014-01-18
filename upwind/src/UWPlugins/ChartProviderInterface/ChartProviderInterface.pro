#-------------------------------------------------
#
# Project created by QtCreator 2011-11-02T15:02:27
#
#-------------------------------------------------

QT += core widgets

TARGET = ChartProviderInterface

TEMPLATE = lib

SOURCES += \
    objects/coastline.cpp \
    objects/rock.cpp \
    objects/limit.cpp \
    objects/deptharea.cpp \
    objects/wreck.cpp \
    objects/navigline.cpp \
    objects/transmline.cpp \
    objects/bridge.cpp \
    objects/signsound.cpp \
    objects/navaid.cpp \
    objects/generarea.cpp \
    objects/depthcontour.cpp \
    objects/depthareashallow.cpp

HEADERS += \
    ../UWPluginInterface/uwplugininterface.h \
    chartproviderinterface.h \
    corechartprovider.h \
    chartobjectinterface.h \
    objects/coastline.h \
    objects/rock.h \
    objects/limit.h \
    objects/deptharea.h \
    objects/wreck.h \
    objects/navigline.h \
    objects/transmline.h \
    objects/bridge.h \
    objects/signsound.h \
    objects/navaid.h \
    objects/depthcontour.h \
    objects/generarea.h \
    objects/depthareashallow.h

DESTDIR += ../uwplugins


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
    LIBS += -L$$PWD/../../../../libs/win/gdal/lib -lgdal-1
}





