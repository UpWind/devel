#include "nmealogger.h"

NMEALogger::NMEALogger(){
    this->initializeSettings();
    settingsUI = new SettingsUI(this);
    settingsUI->setupSettings(settings);
}

NMEALogger::~NMEALogger(){}

/**
  * Adds the plugin to the given layout. The plugin will no longer be on it's own window.
  * @param layout - Pointer to a QLayout to which the plugin widget (UI) will be added
  */
void NMEALogger::addPluginSettingsToLayout(QLayout *layout){
    if(layout != 0)
        layout->addWidget(settingsUI);
}

QString NMEALogger::getName(){
    return "NMEALogger";
}

void NMEALogger::saveBeforeQuit(){
    this->logNMEAEntries();
    if(timedSavingOn)
        this->disableSavelogTimer();

    settingsUI->updateSettings();
}

void NMEALogger::initializeSettings(){
    QFileInfo info(this->getName() + ".xml");
    settings = new Settings(this->getName());

    if(info.exists())
        settings->loadSettings();
    else{
        //construct new settings
        settings->setSetting("GPS", "true");
        settings->setSetting("Anenometer", "true");
        settings->setSetting("Compass", "true");
        settings->setSetting("Clock", "true");
        settings->setSetting("TimedRecording", "true");
        settings->setSetting("RegularRecording", "false");
        settings->setSetting("TimeInterval", "1");
        settings->setSetting("RegularInterval", "20");
    }
}

Q_EXPORT_PLUGIN2(nmealogger, NMEALogger)
