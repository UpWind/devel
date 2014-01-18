QT += core widgets xml svg

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
    ../ChartProviderInterface/objects/signsound.h \
    ../ChartProviderInterface/objects/navaid.h

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
    ../ChartProviderInterface/objects/depthcontour.cpp \
    ../ChartProviderInterface/objects/signsound.cpp \
    ../ChartProviderInterface/objects/navaid.cpp


FORMS += settingsui.ui

DESTDIR += ../plugins

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
    INCLUDEPATH += "$$PWD/../../../../libs/win/gdal/include"
    LIBS += -L$$PWD/../../../../libs/win/gdal/lib -lgdal-1
}

