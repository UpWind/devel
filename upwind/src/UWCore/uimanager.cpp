#include "uimanager.h"
#include "uwcore.h"
#include <QRect>
#include <QGridLayout>
#include <QLayout>

UIManager::UIManager() {
    mainWindow = new MainWindow();
    mainMenu = new MainMenu(mainWindow);
    navigationWindow = new NavigationWindow(mainWindow);
    settingsWindow = new SettingsForm(mainWindow);

    mainWindow->layout()->addWidget(mainMenu);
    mainWindow->layout()->addWidget(navigationWindow);
    mainWindow->layout()->addWidget(settingsWindow);
    toolbox = new ToolBox(navigationWindow);

    QObject::connect(mainWindow, SIGNAL(geometryChanged(QRect)), this, SLOT(mainWindowGeometryChanged(QRect)));
    QObject::connect(navigationWindow, SIGNAL(goBack()), this, SLOT(showMainMenu()));
    QObject::connect(navigationWindow, SIGNAL(toolboxButtonClicked()), this, SLOT(toggleToolbox()));
    QObject::connect(mainMenu, SIGNAL(close()), this, SLOT(close()));
    QObject::connect(mainMenu, SIGNAL(showChart()), this, SLOT(showChartWindow()));
    QObject::connect(mainMenu, SIGNAL(showSettings()), this, SLOT(showSettingsWindow()));
    QObject::connect(settingsWindow, SIGNAL(goBack()), this, SLOT(showMainMenu()));

    connectInstruments();
    mainWindow->show();
}

void UIManager::connectInstruments(){
    //if the instruments are loaded, connect them
    QList<NMEAInstrumentInterface*> instruments = UWCore::getInstance()->getPluginManager()->getInstruments();
    foreach(NMEAInstrumentInterface* instrument,instruments){
        instrument->getGUI()->setParent(navigationWindow);
        instrument->showPlugin();
    }
}

void UIManager::mainWindowGeometryChanged(QRect geometry)
{
    navigationWindow->setGeometry(geometry);
    toolbox->setGeometry((geometry.right() - toolbox->width()), 50,
                         toolbox->width(), geometry.bottom() - 120);
}



SettingsForm* UIManager::getSettingsForm(){
    return settingsWindow;
}

void UIManager::toggleToolbox(){
    if(toolbox->isVisible())
        toolbox->hide();
    else
        toolbox->show();
}

ToolBox* UIManager::getToolBox(){
    return toolbox;
}

NavigationWindow * UIManager::chartDisplayWidget() {
    return navigationWindow;
}

void UIManager::showChartWindow(){
    (qobject_cast<QStackedLayout*>(mainWindow->layout()))->setCurrentWidget(navigationWindow);
}

void UIManager::showMainMenu(){
    UWCore::getInstance()->getPluginManager()->savePluginSettings();
    UWCore::getInstance()->getPluginManager()->saveInstrumentPositions();

    (qobject_cast<QStackedLayout*>(mainWindow->layout()))->setCurrentWidget(mainMenu);
}

void UIManager::showSettingsWindow(){
    (qobject_cast<QStackedLayout*>(mainWindow->layout()))->setCurrentWidget(settingsWindow);
}

void UIManager::close(){
    mainWindow->close();
}

UIManager::~UIManager(){
    delete mainMenu;
    delete settingsWindow;
    delete mainWindow;
    delete navigationWindow;
}
