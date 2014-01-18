#-------------------------------------------------
#
# Project created by QtCreator 2011-03-10T10:54:33
#
#-------------------------------------------------

QT       += core widgets xml sql

TARGET = PostgreSQL
TEMPLATE = lib

SOURCES +=\
        settingsui.cpp \
    pgdatabase.cpp \
    ../ChartReader/corechartreader.cpp \
    ../../Settings/settings.cpp \
    chartEditor/generatorsettingsdialog.cpp \
    chartEditor/loadcharttabledialog.cpp \
    chartEditor/charteditorwindow.cpp \
    shared/uwstatus.cpp \
    shared/uwmath.cpp \
    shared/utils.cpp \
    layers/zoomthread.cpp \
    layers/threadfactory.cpp \
    layers/polygon.cpp \
    layers/point.cpp \
    layers/ogroptimizer.cpp \
    layers/line.cpp \
    layers/LayerVisualInfo.cpp \
    layers/LayersSaver.cpp \
    layers/LayersManager.cpp \
    layers/LayersLoader.cpp \
    layers/layersimplificationhandler.cpp \
    layers/Layer.cpp \
    layers/charttablesettings.cpp \
    layers/ChartTableGen.cpp \
    layers/ChartTable.cpp \
    layers/chart.cpp \
    settings/SettingsManager.cpp \
    settings/ConfigWriter.cpp \
    layers/Lights/Utilies.cpp \
    layers/Lights/Sector.cpp \
    layers/Lights/Lighthouse.cpp \
    layers/Lights/Buoy.cpp \
    shared/polardiagram.cpp



HEADERS  += settingsui.h \
    pgdatabase.h \
    ../ChartReader/ChartReaderInterface.h \
    ../ChartReader/corechartreader.h \
    ../../Settings/settings.h \
    chartEditor/generatorsettingsdialog.h \
    chartEditor/loadcharttabledialog.h \
    chartEditor/charteditorwindow.h \
    shared/uwstatus.h \
    shared/uwmath.h \
    shared/utils.h \
    layers/zoomthread.h \
    layers/threadfactory.h \
    layers/polygon.h \
    layers/point.h \
    layers/ogroptimizer.h \
    layers/line.h \
    layers/LayerVisualInfo.h \
    layers/LayersSaver.h \
    layers/LayersManager.h \
    layers/LayersLoader.h \
    layers/layersimplificationhandler.h \
    layers/Layer.h \
    layers/charttablesettings.h \
    layers/ChartTableGen.h \
    layers/ChartTable.h \
    layers/chart.h \
    settings/SettingsManager.h \
    settings/Device.h \
    settings/ConfigWriter.h \
    layers/Lights/Utilies.h \
    layers/Lights/Sector.h \
    layers/Lights/Lighthouse.h \
    layers/Lights/Light.h \
    layers/Lights/Buoy.h \
    shared/polardiagram.h




DESTDIR         = ../plugins

FORMS    += settingsui.ui \
    chartEditor/generatorsettingsdialog.ui \
    chartEditor/loadcharttabledialog.ui \
    chartEditor/charteditorwindow.ui

mac {
    INCLUDEPATH += /Library/Frameworks/GDAL.framework/Headers/
    LIBS += -L/Library/Frameworks/GDAL.framework/unix/lib -lgdal -lpq
}

win32 {
    INCLUDEPATH += D:\\gdal\\include
    INCLUDEPATH += "C:\\PostgreSQL\\8.3\\include"
    LIBS += -LD:\\gdal\\bin
    LIBS += -llibgdal-1 \
        -llibpq
    LIBS += -L"\\C:\\PostgreSQL\\8.3\\bin"
}

else:unix {

    INCLUDEPATH +=/usr/include/gdal/ \
        /usr/include/postgresql
    LIBS +=-lpq -lgdal
}
