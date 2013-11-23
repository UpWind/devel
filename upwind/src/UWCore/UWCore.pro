#-------------------------------------------------
#
# Project created by QtCreator 2011-02-08T08:45:37
#
#-------------------------------------------------
CONFIG += serialport
QT += serialport
QT += widgets

QT += core gui\
    network \
    xml\
    webkit\
    svg

unix:INCLUDEPATH += "/usr/include/postgresql"
unix:INCLUDEPATH += "/usr/include/gdal/"

TARGET = UWCore
TEMPLATE = app

SOURCES += main.cpp \
    uwcore.cpp \
    uimanager.cpp \
    pluginmanager.cpp \
    filemanager.cpp \
    settingsmanager.cpp \
    UI/mainmenu.cpp \
    UI/settingswindow.cpp \
    UI/mainwindow.cpp \
    UI/settingsform.cpp \
    ../Settings/settings.cpp \
    UI/ToolBox/toolbox.cpp \
    UI/ToolBox/tab.cpp \
    UI/pluginselect.cpp \
    ../UWPlugins/NMEAReader/corenmeareader.cpp \
    ../UWPlugins/ViewRenderer/coreviewrenderer.cpp \
    UI/navigationwindow.cpp \

#UI/coastline.cpp

HEADERS += \
    uwcore.h \
    uimanager.h \
    pluginmanager.h \
    filemanager.h \
    settingsmanager.h \
    ../Settings/settings.h \
    ../UWPlugins/ChartProviderInterface/chartproviderinterface.h \
    ../UWPlugins/ChartProviderInterface/corechartprovider.h \
    ../UWPlugins/ViewRenderer/ViewRendererInterface.h \
    ../UWPlugins/UWPluginInterface/UWPluginInterface.h \
    ../UWPlugins/UpWindScene/UpWindSceneInterface.h \
    ../UWPlugins/Logger/LoggerInterface.h \
    ../UWPlugins/RouteManager/RouteManagerInterface.h \
    ../UWPlugins/NMEAInstruments/CoreNMEAInstrument/NMEAInstrumentInterface.h \
    ../UWPlugins/NMEAReader/corenmeareader.h \
    ../UWPlugins/ViewRenderer/coreviewrenderer.h \
    UI/mainmenu.h\
    UI/settingswindow.h \
    UI/mainwindow.h \
    UI/settingsform.h \
    UI/ToolBox/toolbox.h \
    UI/ToolBox/tab.h \
    UI/pluginselect.h \
    UI/navigationwindow.h \
    ../UWPlugins/NMEAReader/NMEAReaderInterface.h

#UI/coastline.h

FORMS += \
    UI/mainmenu.ui \
    UI/settingswindow.ui \
    UI/settingsform.ui \
    UI/ToolBox/toolBox.ui \
    UI/pluginselect.ui

RESOURCES += \
    resources/resources.qrc

INCLUDEPATH += ../UWPlugins \
    ../UWPlugins/ChartProviderInterface

win32 {
#  DESTDIR = C:/myQt/bin
#  MOC_DIR      = C:/tmp/moc
#  OBJECTS_DIR  = C:/tmp/obj
    INCLUDEPATH += "$$PWD/../../../libs/win/PostgreSQL9.2/include"
    LIBS += -L$$PWD/../../../../libs/win/PostgreSQL9.2/lib -lpq

    INCLUDEPATH += "$$PWD/../../../../libs/win/gdal/include"
    LIBS += -L$$PWD/../../../../libs/win/gdal/lib -lgdal-1
}









