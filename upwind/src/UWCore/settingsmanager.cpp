#include "settingsmanager.h"

#include <iostream>
#include <QFileInfo>
#include <QDebug>

SettingsManager::SettingsManager(){
    this->initializeSettings();
}

SettingsManager::~SettingsManager(){
    delete selectedPlugins;
}

void SettingsManager::initializeSettings(){
    QFileInfo info("SelectedPlugins.xml");
    qDebug() << Q_FUNC_INFO << info.absoluteFilePath();
    selectedPlugins = new Settings("SelectedPlugins");

    if(info.exists()){
        selectedPlugins->loadSettings();
    }
    else{
        //construct a new settings
        selectedPlugins->setSetting("Renderer","");
        selectedPlugins->setSetting("NMEAReader","");
        selectedPlugins->setSetting("ChartProvider","");
        selectedPlugins->setSetting("Logger","");
        selectedPlugins->setSetting("RouteManager","");
        selectedPlugins->setSetting("UpWindScene","");
    }
}

Settings * SettingsManager::getSelectedPlugins(){
    return selectedPlugins;
}
