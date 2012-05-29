#ifndef COREINTERFACE_H
#define COREINTERFACE_H

#include <QtGui>
#include <QObject>

/**
 * CoreInterface class is the plugin interface for all used plugins. It contains the
 * necessary methods for using the plugins.
 */
class CoreInterface
{
public:
        /**
          * Virtual destructor of the interface class
          */
	virtual ~CoreInterface() {}
        /**
          * Creates the plugin's GUI and it's elements
          */
        virtual void CreateGUI() = 0;
        /**
          * Shows the GUI of the plugin
          */
        virtual void showPlugin() = 0;
        /**
         * Hides the GUI of the plugin
         */
        virtual void hidePlugin() = 0;
        /**
          * Adds the plugin to the given layout. The plugin will no longer be on it's own window.
          * @param layout - Pointer to a QLayout to which the plugin widget (UI) will be added
          */
        virtual void addPluginToLayout(QLayout *layout) = 0;
        /**
          * Removes the plugin from the given layout. The plugin will be on it's own window.
          * @param layout - Pointer to a QLayout from which the plugin widget (UI) will be removed
          */
        virtual void removePluginFromLayout(QLayout *layout) = 0;
        /**
         * Parses an NMEAString, loading the values from it and updating the GUI
         * @param text - Reference to the QString which will be parsed
         */
        virtual void parseNMEAString( const QString & text) = 0;
        /**
          * Returns the name of the plugin
          * @return name of the plugin as a QString
          */
        virtual QString getName() = 0;
        /**
          * Returns the message IDs of the messages the plugin produces
          * @return message IDs separated by a semicolong (;).
          */
        virtual QString getProduceIDs() = 0;
        /**
          * Returns the message IDs of the messages the plugin can parse
          * @return message IDs separated by a semicolong (;).
          */
        virtual QString getParseIDs() = 0;
        /**
          * Starts the plugin's internal timer.
          */
        virtual void start() = 0;
        /**
          * Stops the plugin's internal timer.
          */
        virtual void stop() = 0;
};

Q_DECLARE_INTERFACE(CoreInterface,"com.Upwind.CoreInterface/1.0");

#endif // COREINTERFACE_H
