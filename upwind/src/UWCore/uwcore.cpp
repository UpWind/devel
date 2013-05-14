#include "uwcore.h"

UWCore *UWCore::instance = 0;

UWCore::UWCore(){}

void UWCore::initialize(){
    settingsManager = new SettingsManager();
    pluginManager = new PluginManager();
    fileManager = new FileManager();
    pluginManager->loadPlugins();
    uiManager = new UIManager();

    // tämä on väärin
    // tämä yhdistää SLOTIT ja SIGNALIT
    // jotain
    pluginManager->makeConnections();

    // TODO tässä vaiheessa voisi connectata pluginit keskenään.

}

UWCore::~UWCore(){
    delete uiManager;
    delete settingsManager;
    delete fileManager;
    delete pluginManager;
}

UIManager* UWCore::getUIManager(){
    return uiManager;
}

SettingsManager* UWCore::getSettingsManager(){
    return settingsManager;
}

FileManager* UWCore::getFileManager(){
    return fileManager;
}

PluginManager* UWCore::getPluginManager(){
    return pluginManager;
}
