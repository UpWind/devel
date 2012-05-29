#ifndef NMEALOGGER_H
#define NMEALOGGER_H

#include <QObject>
#include "../Logger/corelogger.h"
#include "settingsui.h"

/** Class declaration of NMEALogger. Inherits CoreLogger
  */

class NMEALogger: public CoreLogger
{
    Q_OBJECT
public:
    NMEALogger();
    ~NMEALogger();
    /**
      * Adds the plugin to the given layout. The plugin will no longer be on it's own window.
      * @param layout - Pointer to a QLayout to which the plugin widget (UI) will be added
      */
    void addPluginSettingsToLayout(QLayout *layout) ;

    /** Initializes settings for NMEALogger unless settings file is already created
      */
    void initializeSettings();

    /** Called when program is about to quit or the plugin is about the be detached
      */
    void saveBeforeQuit();

    /** Gets the name of plugin
      * @return QString
      */
    QString getName();

private:
    SettingsUI* settingsUI;
};

#endif // NMEALOGGER_H
