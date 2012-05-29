#ifndef UWCORE_H
#define UWCORE_H

#include "uimanager.h"
#include "settingsmanager.h"
#include "filemanager.h"
#include "pluginmanager.h"

class UWCore
{

public:

    /**
     * Getter for the singleton UWCore.
     * @return the singleton instance of UWCore.
     */
    static UWCore * getInstance(){
        if(instance == 0)
            instance = new UWCore();
        return instance;
    }

    /**
     * Destructor of the UWCore.
     */
    ~UWCore();

    /**
     * Getter for the plugin manager.
     * @return pointer to the plugin manager.
     */
    PluginManager * getPluginManager();

    /**
     * Getter for the UIManager.
     * @return pointer to the UIManager.
     */
    UIManager * getUIManager();

    /**
     * Getter for the settings manager.
     * @return pointer to the settings manager.
     */
    SettingsManager * getSettingsManager();

    /**
     * Getter for the file manager.
     * @return pointer to the file manager.
     */
    FileManager * getFileManager();

    /**
     * UWCore initialization.
     */
    void initialize();

private:

    /**
     * Private constructor of the UWCore.
     */
    UWCore();

    static UWCore *instance;
    UIManager *uiManager;
    SettingsManager *settingsManager;
    FileManager *fileManager;
    PluginManager *pluginManager;
};

#endif
