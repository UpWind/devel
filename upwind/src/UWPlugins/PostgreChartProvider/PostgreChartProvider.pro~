QT += core gui xml svg

TEMPLATE = lib

INCLUDEPATH += \
    ../ChartProviderInterface \
    ../ChartProviderInterface/objects \
    ../../Settings \
    ../shared

HEADERS += \
    postgrechartprovider.h \
    settingsui.h \
    ../shared/uwmath.h \
    ../../Settings/settings.h \
    ../ChartProviderInterface/chartproviderinterface.h \
    ../ChartProviderInterface/corechartprovider.h \
    ../ChartProviderInterface/objects/coastline.h \
    ../ChartProviderInterface/objects/deptharea.h \
    ../ChartProviderInterface/objects/depthareashallow.h \
    ../ChartProviderInterface/objects/rock.h \
    ../ChartProviderInterface/objects/wreck.h \
    ../ChartProviderInterface/objects/limit.h \
    ../ChartProviderInterface/objects/navigline.h \
    ../ChartProviderInterface/objects/transmline.h \
    ../ChartProviderInterface/objects/bridge.h \
    ../ChartProviderInterface/objects/depthcontour.h \

SOURCES += \
    postgrechartprovider.cpp \
    settingsui.cpp \
    ../../Settings/settings.cpp \
    ../shared/uwmath.cpp \
    ../ChartProviderInterface/objects/coastline.cpp \
    ../ChartProviderInterface/objects/deptharea.cpp \
    ../ChartProviderInterface/objects/depthareashallow.cpp \
    ../ChartProviderInterface/objects/rock.cpp \
    ../ChartProviderInterface/objects/wreck.cpp \
    ../ChartProviderInterface/objects/limit.cpp \
    ../ChartProviderInterface/objects/navigline.cpp \
    ../ChartProviderInterface/objects/transmline.cpp \
    ../ChartProviderInterface/objects/bridge.cpp \
    ../ChartProviderInterface/objects/depthcontour.cpp


FORMS += settingsui.ui

DESTDIR += ../plugins

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
