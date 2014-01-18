#ifndef UWPLUGININTERFACE_H
#define UWPLUGININTERFACE_H

#include <QtWidgets>
#include "../../Settings/settings.h"

/**
 * CoreInterface class is the plugin interface for all used plugins. It contains the
 * necessary methods for using the plugins.
 */
class UWPluginInterface
{
public:
    /** Virtual destructor of the interface class
      */
    virtual ~UWPluginInterface() {}

    /** Adds the plugin to the given layout. The plugin will no longer be on it's own window.
      * @param layout - Pointer to a QLayout to which the plugin widget (UI) will be added
      */
    virtual void addPluginSettingsToLayout(QLayout *layout) = 0;

    /** Returns the name of the plugin
      * @return name of the plugin as a QString
      */
    virtual QString getName() = 0;

    virtual Settings * getSettings() = 0;
};

Q_DECLARE_INTERFACE(UWPluginInterface,"com.Upwind.UWPluginInterface/1.0")
#endif // UWPLUGININTERFACE_H
