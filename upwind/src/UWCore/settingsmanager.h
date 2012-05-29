#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include "../Settings/settings.h"

/*! \file settingsmanager.h
    \brief This file includes SettingsManager which is used for managing setting saving and loading
   */

//! SettingsManager class
/*!
  This class manages the way the settings of the application and plugins are saved and loaded. This uses
  XML-file for keeping the settings.
*/
class SettingsManager : public QObject
{
    Q_OBJECT
public:

    /**
     * Constructor of the SettingsManager.
     */
    explicit SettingsManager();

    /**
     * Destructor of the SettingsManager.
     */
    ~SettingsManager();

    /**
     * Getter for the selected plugins settings.
     * @return pointer to the settings object.
     */
    Settings * getSelectedPlugins();

private:
    /**
     * Initializes the settings.
     */
    void initializeSettings();
    Settings* selectedPlugins;
};
#endif // SETTINGSMANAGER_H
