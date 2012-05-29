#include "uimanager.h"
#include "uwcore.h"
#include <QRect>
#include <QGridLayout>
#include <QLayout>

UIManager::UIManager(){
    mainWindow = new MainWindow();
    mainMenu = new MainMenu(mainWindow);
    mainWindow->layout()->addWidget(mainMenu);
    navigationWindow = new NavigationWindow(mainWindow);
    navigationWindow->layout()->setEnabled(false);
    navigationWindow->hide();

    mainWindow->layout()->addWidget(navigationWindow);
    mainWindow->show();

    QObject::connect(navigationWindow, SIGNAL(goBack()), this, SLOT(showMainMenu()));
    QObject::connect(navigationWindow, SIGNAL(toggleToolbox()), this, SLOT(toggleToolbox()));
    QObject::connect(mainMenu, SIGNAL(close()), this, SLOT(close()));
    QObject::connect(mainMenu, SIGNAL(showChart()), this, SLOT(showChartWindow()));
    QObject::connect(mainMenu, SIGNAL(showSettings()), this, SLOT(showSettingsWindow()));
}

void UIManager::initialize(){
    settingsWindow = new SettingsForm(mainWindow);

    mainWindow->layout()->addWidget(settingsWindow);
    settingsWindow->hide();
    QObject::connect(settingsWindow, SIGNAL(goBack()), this, SLOT(showMainMenu()));

    toolbox = new ToolBox(navigationWindow);

//    Antti: Why not get the real screen geometry? not window geometry
//    QRect screenGeometry = navigationWindow->geometry();
    QRect screenGeometry =  QApplication::desktop()->screenGeometry();

    toolbox->setGeometry((screenGeometry.topRight().x() - toolbox->width()), 50, toolbox->width(), screenGeometry.bottomRight().y() - 120);
    navigationWindow->addButtons();

    connectInstruments();
}

void UIManager::connectInstruments(){
    //if the instruments are loaded, connect them
    QList<NMEAInstrumentInterface*> instruments = UWCore::getInstance()->getPluginManager()->getInstruments();
    foreach(NMEAInstrumentInterface* instrument,instruments){
        navigationWindow->layout()->addWidget(instrument->getGUI());
        instrument->showPlugin();
    }
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

NavigationWindow * UIManager::getMap(){
    return navigationWindow;
}

void UIManager::showChartWindow(){
    navigationWindow->show();
}

void UIManager::showMainMenu(){
    UWCore::getInstance()->getPluginManager()->savePluginSettings();
    UWCore::getInstance()->getPluginManager()->saveInstrumentPositions();

    mainMenu->show();
}

void UIManager::showSettingsWindow(){
    settingsWindow->showSettings();
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
